/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           |
     \\/     M anipulation  |
-------------------------------------------------------------------------------
                            | Copyright (C) 2011-2017 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport
(
    const Thermo& t,
    const vector kappa,
    const vector sigmaE
)
:
    Thermo(t),
    kappa_(kappa),
    sigmaE_(sigmaE)
{}


template<class Thermo>
inline Foam::constAnIsoSolidTransport<Thermo>::constAnIsoSolidTransport
(
    const word& name,
    const constAnIsoSolidTransport& ct
)
:
    Thermo(name, ct),
    kappa_(ct.kappa_),
    sigmaE_(ct.sigmaE_)
{}


template<class Thermo>
inline Foam::autoPtr<Foam::constAnIsoSolidTransport<Thermo>>
Foam::constAnIsoSolidTransport<Thermo>::New
(
    const dictionary& dict
)
{
    return autoPtr<constAnIsoSolidTransport<Thermo>>::New(dict);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
inline Foam::scalar Foam::constAnIsoSolidTransport<Thermo>::
kappa(const scalar p, const scalar T) const
{
    return mag(kappa_);
}

template<class Thermo>
inline Foam::vector Foam::constAnIsoSolidTransport<Thermo>::
Kappa(const scalar p, const scalar T) const
{
    return kappa_;
}

template<class Thermo>
inline Foam::scalar Foam::constAnIsoSolidTransport<Thermo>::
sigmaE(const scalar p, const scalar T) const
{
    return mag(sigmaE_);
}

template<class Thermo>
inline Foam::vector Foam::constAnIsoSolidTransport<Thermo>::
SigmaE(const scalar p, const scalar T) const
{
    return sigmaE_;
}


template<class Thermo>
inline Foam::scalar Foam::constAnIsoSolidTransport<Thermo>::
mu(const scalar p, const scalar T) const
{
    NotImplemented;
    return scalar(0);
}


template<class Thermo>
inline Foam::vector Foam::constAnIsoSolidTransport<Thermo>::
alphah(const scalar p, const scalar T) const
{
    return kappa_/this->Cp(p, T);
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Thermo>
inline void Foam::constAnIsoSolidTransport<Thermo>::operator=
(
    const constAnIsoSolidTransport<Thermo>& ct
)
{
    kappa_ = ct.kappa_;
    sigmaE_ = ct.sigmaE_;
}


template<class Thermo>
inline void Foam::constAnIsoSolidTransport<Thermo>::operator+=
(
    const constAnIsoSolidTransport<Thermo>& ct
)
{
    scalar Y1 = this->Y();

    Y1 /= this->Y();
    scalar Y2 = ct.Y()/this->Y();

    kappa_ = Y1*kappa_ + Y2*ct.kappa_;
    sigmaE_ = Y1*sigmaE_ + Y2*ct.sigmaE_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


template<class Thermo>
inline Foam::constAnIsoSolidTransport<Thermo> Foam::operator*
(
    const scalar s,
    const constAnIsoSolidTransport<Thermo>& ct
)
{
    return constAnIsoSolidTransport<Thermo>
    (
        s*static_cast<const Thermo&>(ct),
        ct.kappa_,
	ct.sigmaE_
    );
}

// ************************************************************************* //
