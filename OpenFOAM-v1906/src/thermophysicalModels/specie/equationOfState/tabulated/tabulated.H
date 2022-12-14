/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2012 OpenFOAM Foundation
     \\/     M anipulation  |
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
    Foam::tabulated

Description
    Tabulated equation of state.

SourceFiles
    tabulatedI.H
    tabulated.C

\*---------------------------------------------------------------------------*/

#ifndef tabulated_H
#define tabulated_H

#include "autoPtr.H"
// #include "interpolation2DTable.H"
#include "interpolationTable.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators

template<class Specie> class tabulated;

template<class Specie>
Ostream& operator<<
(
    Ostream&,
    const tabulated<Specie>&
);


/*---------------------------------------------------------------------------*\
                           Class tabulated Declaration
\*---------------------------------------------------------------------------*/

template<class Specie>
class tabulated
:
    public Specie
{

        //- Constant dynamic viscosity [Pa.s]
//         interpolation2DTable<scalar> rhoTable_;
		interpolationTable<scalar> rhoTable_;

        //- Reciprocal Prandtl Number []
//         interpolation2DTable<scalar> CpMCvTable_;
public:

    // Constructors

        //- Construct from components
        inline tabulated(const Specie& sp);

        //- Construct from Istream
        tabulated(Istream&);

        //- Construct from dictionary
        tabulated(const dictionary& dict);

        //- Construct as named copy
        inline tabulated(const word& name, const tabulated&);

        //- Construct and return a clone
        inline autoPtr<tabulated> clone() const;

        // Selector from Istream
        inline static autoPtr<tabulated> New(Istream& is);

        // Selector from dictionary
        inline static autoPtr<tabulated> New(const dictionary& dict);


    // Member functions

        //- Return the instantiated type name
        static word typeName()
        {
            return "tabulated<" + word(Specie::typeName_()) + '>';
        }


        // Fundamental properties

            //- Is the equation of state is incompressible i.e. rho != f(p)
            static const bool incompressible = false;

            //- Is the equation of state is isochoric i.e. rho = const
            static const bool isochoric = false;

			//- Return density [kg/m^3]
			inline scalar rho(scalar p, scalar T) const;
			
			//- Return enthalpy departure [J/kg]
			inline scalar H(const scalar p, const scalar T) const;
			
			//- Return Cp departure [J/(kg K]
			inline scalar Cp(scalar p, scalar T) const;
			
			//- Return internal energy departure [J/kg]
			inline scalar E(const scalar p, const scalar T) const;
			
			//- Return Cv departure [J/(kg K]
			inline scalar Cv(scalar p, scalar T) const;
			
			//- Return entropy [J/(kg K)]
			inline scalar S(const scalar p, const scalar T) const;
			
			//- Return compressibility rho/p [s^2/m^2]
			inline scalar psi(scalar p, scalar T) const;
			
			//- Return compression factor []
			inline scalar Z(scalar p, scalar T) const;
			
			//- Return (Cp - Cv) [J/(kg K]
			inline scalar CpMCv(scalar p, scalar T) const;


        // IO

            //- Write to Ostream
            void write(Ostream& os) const;


    // Member operators

        inline void operator+=(const tabulated&);
        inline void operator-=(const tabulated&);
        inline void operator*=(const scalar);




    // Ostream Operator

        friend Ostream& operator<< <Specie>
        (
            Ostream&,
            const tabulated&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "tabulatedI.H"

#ifdef NoRepository
#   include "tabulated.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
