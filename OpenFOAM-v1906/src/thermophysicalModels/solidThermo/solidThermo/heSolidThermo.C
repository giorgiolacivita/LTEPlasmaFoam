/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2017-2018 OpenCFD Ltd
     \\/     M anipulation  |
-------------------------------------------------------------------------------
                            | Copyright (C) 2011-2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "heSolidThermo.H"
#include "volFields.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class BasicSolidThermo, class MixtureType>
void Foam::heSolidThermo<BasicSolidThermo, MixtureType>::calculate()
{
    scalarField& TCells = this->T_.primitiveFieldRef();

    const scalarField& hCells = this->he_;
    const scalarField& pCells = this->p_;
    scalarField& rhoCells = this->rho_.primitiveFieldRef();
    //scalarField& psiCells = this->psi_.primitiveFieldRef();
    //scalarField& muCells = this->mu_.primitiveFieldRef();
    scalarField& alphaCells = this->alpha_.primitiveFieldRef();
    scalarField& sigmaECells = this->sigmaE_.primitiveFieldRef();

    forAll(TCells, celli)
    {
        const typename MixtureType::thermoType& mixture_ =
            this->cellMixture(celli);

        const typename MixtureType::thermoType& volMixture_ =
            this->cellVolMixture(pCells[celli], TCells[celli], celli);

        if (this->updateT())
        {
            TCells[celli] = mixture_.THE
            (
                hCells[celli],
                pCells[celli],
                TCells[celli]
            );
        }

        rhoCells[celli] = volMixture_.rho(pCells[celli], TCells[celli]);
        //psiCells[celli] = volMixture_.psi(pCells[celli], TCells[celli]);
        //muCells[celli] = volMixture_.mu(pCells[celli], TCells[celli]);
	sigmaECells[celli] = volMixture_.sigmaE(pCells[celli], TCells[celli]);

        alphaCells[celli] =
            volMixture_.kappa(pCells[celli], TCells[celli])
            /
            mixture_.Cpv(pCells[celli], TCells[celli]);
    }

    volScalarField::Boundary& pBf = this->p_.boundaryFieldRef();
    volScalarField::Boundary& TBf = this->T_.boundaryFieldRef();
    volScalarField::Boundary& rhoBf = this->rho_.boundaryFieldRef();
    //volScalarField::Boundary& psiBf = this->psi_.boundaryFieldRef();
    //volScalarField::Boundary& muBf = this->mu_.boundaryFieldRef();
    volScalarField::Boundary& heBf = this->he().boundaryFieldRef();
    volScalarField::Boundary& sigmaEBf = this->sigmaE_.boundaryFieldRef();
    volScalarField::Boundary& alphaBf = this->alpha_.boundaryFieldRef();

    forAll(this->T_.boundaryField(), patchi)
    {
        fvPatchScalarField& pp = pBf[patchi];
        fvPatchScalarField& pT = TBf[patchi];
        fvPatchScalarField& prho = rhoBf[patchi];
        //fvPatchScalarField& ppsi = psiBf[patchi];
        //fvPatchScalarField& pmu = muBf[patchi];
        fvPatchScalarField& phe = heBf[patchi];
	fvPatchScalarField& psigmaE = sigmaEBf[patchi];
        fvPatchScalarField& palpha = alphaBf[patchi];

        if (pT.fixesValue())
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                const typename MixtureType::thermoType& volMixture_ =
                    this->patchFaceVolMixture
                    (
                        pp[facei],
                        pT[facei],
                        patchi,
                        facei
                    );


                phe[facei] = mixture_.HE(pp[facei], pT[facei]);
                prho[facei] = volMixture_.rho(pp[facei], pT[facei]);
                //ppsi[facei] = volMixture_.psi(pp[facei], pT[facei]);
                //pmu[facei] = volMixture_.mu(pp[facei], pT[facei]);
		psigmaE[facei] = volMixture_.sigmaE(pp[facei], pT[facei]);

                palpha[facei] =
                    volMixture_.kappa(pp[facei], pT[facei])
                  / mixture_.Cpv(pp[facei], pT[facei]);
            }
        }
        else
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                const typename MixtureType::thermoType& volMixture_ =
                    this->patchFaceVolMixture
                    (
                        pp[facei],
                        pT[facei],
                        patchi,
                        facei
                    );

                if (this->updateT())
                {
                    pT[facei] = mixture_.THE(phe[facei], pp[facei] ,pT[facei]);
                }

                prho[facei] = volMixture_.rho(pp[facei], pT[facei]);
                //ppsi[facei] = volMixture_.psi(pp[facei], pT[facei]);
                //pmu[facei] = volMixture_.mu(pp[facei], pT[facei]);
		psigmaE[facei] = volMixture_.sigmaE(pp[facei], pT[facei]);

                palpha[facei] =
                    volMixture_.kappa(pp[facei], pT[facei])
                  / mixture_.Cpv(pp[facei], pT[facei]);
            }
        }
    }

    this->alpha_.correctBoundaryConditions();
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class BasicSolidThermo, class MixtureType>
Foam::heSolidThermo<BasicSolidThermo, MixtureType>::
heSolidThermo
(
    const fvMesh& mesh,
    const word& phaseName
)
:
    heThermo<BasicSolidThermo, MixtureType>(mesh, phaseName)
{
    calculate();
    this->mu_ == dimensionedScalar(this->mu_.dimensions(), Zero);
    this->psi_ == dimensionedScalar(this->psi_.dimensions(), Zero);
}


template<class BasicSolidThermo, class MixtureType>
Foam::heSolidThermo<BasicSolidThermo, MixtureType>::
heSolidThermo
(
    const fvMesh& mesh,
    const dictionary& dict,
    const word& phaseName
)
:
    heThermo<BasicSolidThermo, MixtureType>(mesh, dict, phaseName)
{
    calculate();
    this->mu_ == dimensionedScalar(this->mu_.dimensions(), Zero);
    this->psi_ == dimensionedScalar(this->psi_.dimensions(), Zero);
}


template<class BasicSolidThermo, class MixtureType>
Foam::heSolidThermo<BasicSolidThermo, MixtureType>::
heSolidThermo
(
    const fvMesh& mesh,
    const word& phaseName,
    const word& dictName
)
:
    heThermo<BasicSolidThermo, MixtureType>(mesh, phaseName, dictName)
{
    calculate();

    // TBD. initialise psi, mu (at heThermo level) since these do not
    // get initialised. Move to heThermo constructor?
    this->mu_ == dimensionedScalar(this->mu_.dimensions(), Zero);
    this->psi_ == dimensionedScalar(this->psi_.dimensions(), Zero);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class BasicSolidThermo, class MixtureType>
Foam::heSolidThermo<BasicSolidThermo, MixtureType>::~heSolidThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class BasicSolidThermo, class MixtureType>
void Foam::heSolidThermo<BasicSolidThermo, MixtureType>::correct()
{
    if (debug)
    {
        InfoInFunction << endl;
    }

    calculate();

    if (debug)
    {
        Info<< "    Finished" << endl;
    }
}


template<class BasicSolidThermo, class MixtureType>
Foam::tmp<Foam::volVectorField>
Foam::heSolidThermo<BasicSolidThermo, MixtureType>::Kappa() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volVectorField> tKappa
    (
        new volVectorField
        (
            IOobject
            (
                "Kappa",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimEnergy/dimTime/dimLength/dimTemperature
        )
    );

    volVectorField& Kappa = tKappa.ref();
    vectorField& KappaCells = Kappa.primitiveFieldRef();
    const scalarField& TCells = this->T_;
    const scalarField& pCells = this->p_;

    forAll(KappaCells, celli)
    {
        Kappa[celli] =
            this->cellVolMixture
            (
                pCells[celli],
                TCells[celli],
                celli
            ).Kappa(pCells[celli], TCells[celli]);
    }

    volVectorField::Boundary& KappaBf = Kappa.boundaryFieldRef();

    forAll(KappaBf, patchi)
    {
        vectorField& Kappap = KappaBf[patchi];
        const scalarField& pT = this->T_.boundaryField()[patchi];
        const scalarField& pp = this->p_.boundaryField()[patchi];

        forAll(Kappap, facei)
        {
            Kappap[facei] =
                this->patchFaceVolMixture
                (
                    pp[facei],
                    pT[facei],
                    patchi,
                    facei
                ).Kappa(pp[facei], pT[facei]);
        }
    }

    return tKappa;
}


template<class BasicSolidThermo, class MixtureType>
Foam::tmp<Foam::vectorField>
Foam::heSolidThermo<BasicSolidThermo, MixtureType>::Kappa
(
    const label patchi
) const
{
    const scalarField& pp = this->p_.boundaryField()[patchi];
    const scalarField& Tp = this->T_.boundaryField()[patchi];
    tmp<vectorField> tKappa(new vectorField(pp.size()));

    vectorField& Kappap = tKappa.ref();

    forAll(Tp, facei)
    {
        Kappap[facei] =
            this->patchFaceVolMixture
            (
                pp[facei],
                Tp[facei],
                patchi,
                facei
            ).Kappa(pp[facei], Tp[facei]);
    }

    return tKappa;
}

template<class BasicSolidThermo, class MixtureType>
Foam::tmp<Foam::volVectorField>
Foam::heSolidThermo<BasicSolidThermo, MixtureType>::SigmaE() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volVectorField> tSigmaE
    (
        new volVectorField
        (
            IOobject
            (
                "SigmaE",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimTime*dimTime*dimTime*dimCurrent*dimCurrent/dimLength/dimLength/dimLength/dimMass
        )
    );

    volVectorField& SigmaE = tSigmaE.ref();
    vectorField& SigmaECells = SigmaE.primitiveFieldRef();
    const scalarField& TCells = this->T_;
    const scalarField& pCells = this->p_;

    forAll(SigmaECells, celli)
    {
        SigmaE[celli] =
            this->cellVolMixture
            (
                pCells[celli],
                TCells[celli],
                celli
            ).SigmaE(pCells[celli], TCells[celli]);
    }

    volVectorField::Boundary& SigmaEBf = SigmaE.boundaryFieldRef();

    forAll(SigmaEBf, patchi)
    {
        vectorField& SigmaEp = SigmaEBf[patchi];
        const scalarField& pT = this->T_.boundaryField()[patchi];
        const scalarField& pp = this->p_.boundaryField()[patchi];

        forAll(SigmaEp, facei)
        {
            SigmaEp[facei] =
                this->patchFaceVolMixture
                (
                    pp[facei],
                    pT[facei],
                    patchi,
                    facei
                ).SigmaE(pp[facei], pT[facei]);
        }
    }

    return tSigmaE;
}


template<class BasicSolidThermo, class MixtureType>
Foam::tmp<Foam::vectorField>
Foam::heSolidThermo<BasicSolidThermo, MixtureType>::SigmaE
(
    const label patchi
) const
{
    const scalarField& pp = this->p_.boundaryField()[patchi];
    const scalarField& Tp = this->T_.boundaryField()[patchi];
    tmp<vectorField> tSigmaE(new vectorField(pp.size()));

    vectorField& SigmaEp = tSigmaE.ref();

    forAll(Tp, facei)
    {
        SigmaEp[facei] =
            this->patchFaceVolMixture
            (
                pp[facei],
                Tp[facei],
                patchi,
                facei
            ).SigmaE(pp[facei], Tp[facei]);
    }

    return tSigmaE;
}


// ************************************************************************* //
