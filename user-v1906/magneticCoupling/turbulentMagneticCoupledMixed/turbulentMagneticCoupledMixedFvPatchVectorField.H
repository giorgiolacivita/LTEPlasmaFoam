/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011, 2017 OpenCFD Ltd.
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

Class
    Foam::compressible::
        turbulentMagneticCoupledMixedFvPatchVectorField

Description
    Mixed boundary condition for temperature and radiation heat transfer
    to be used for in multiregion cases. Optional thin thermal layer
    resistances can be specified through thicknessLayers and kappaLayers
    entries.

    The thermal conductivity \c kappa can either be retrieved from various
    possible sources, as detailed in the class magneticCoupledBase.

Usage
    \table
        Property     | Description             | Required    | Default value
        Tnbr         | name of the field    | no | T
        qrNbr      | name of the radiative flux in the nbr region | no | none
        qr         | name of the radiative flux in this region | no | none
        thicknessLayers | list of thicknesses per layer [m] | no |
        kappaLayers  | list of thermal conductivites per layer [W/m/K] | no |
        kappaMethod  | inherited from magneticCoupledBase | inherited |
        kappa        | inherited from magneticCoupledBase | inherited |
        thermalInertia | Add thermal inertia to wall node | no | false
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            compressible::turbulentMagneticCoupledMixed;
        Tnbr            T;
        qrNbr           qr; // or none. Name of qr field on neighbour region
        qr              qr; // or none. Name of qr field on local region
        thicknessLayers (0.1 0.2 0.3 0.4);
        kappaLayers     (1 2 3 4);
        thermalInertia  false/true;
        kappaMethod     lookup;
        kappa           kappa;
        value           uniform 300;
    }
    \endverbatim

    Needs to be on underlying mapped(Wall)FvPatch.

See also
    Foam::magneticCoupledBase

SourceFiles
    turbulentMagneticCoupledMixedFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef turbulentMagneticCoupledMixedFvPatchVectorField_H
#define turbulentMagneticCoupledMixedFvPatchVectorField_H

#include "mixedFvPatchFields.H"
#include "magneticCoupledBase.H"
#include "scalarList.H"
// #include "vectorList.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace compressible
{

/*---------------------------------------------------------------------------*\
    Class turbulentMagneticCoupledMixedFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class turbulentMagneticCoupledMixedFvPatchVectorField
:
    public mixedFvPatchVectorField,
    public magneticCoupledBase
{
    // Private data

		//- Name of field on the neighbour region
        const word AnbrName_;

        //- Thickness of layers
        scalarList thicknessLayers_;

        //- Conductivity of layers
        scalarList sigmaELayers_;

        //- Total contact resistance
        scalar contactRes_;

//         //- Thermal inertia term
//         Switch thermalInertia_;


public:

    //- Runtime type information
    TypeName("compressible::turbulentMagneticCoupledMixed");


    // Constructors

        //- Construct from patch and internal field
        turbulentMagneticCoupledMixedFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        turbulentMagneticCoupledMixedFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  turbulentTemperatureCoupledBaffleMixedFvPatchVectorField onto a
        //  new patch
        turbulentMagneticCoupledMixedFvPatchVectorField
        (
            const
            turbulentMagneticCoupledMixedFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchVectorField> clone() const
        {
            return tmp<fvPatchVectorField>
            (
                new turbulentMagneticCoupledMixedFvPatchVectorField
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        turbulentMagneticCoupledMixedFvPatchVectorField
        (
            const turbulentMagneticCoupledMixedFvPatchVectorField&,
            const DimensionedField<vector, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new turbulentMagneticCoupledMixedFvPatchVectorField
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
