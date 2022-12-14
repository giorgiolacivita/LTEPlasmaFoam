/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "tabulatedTransport.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Thermo>
Foam::tabulatedTransport<Thermo>::tabulatedTransport(Istream& is)
:
    Thermo(is),
    muTable_(readScalar(is)),
    kappaTable_(readScalar(is)),
    sigmaETable_(readScalar(is))
{
    is.check("tabulatedTransport::tabulatedTransport(Istream& is)");
}


template<class Thermo>
Foam::tabulatedTransport<Thermo>::tabulatedTransport(const dictionary& dict)
:
    Thermo(dict),
    muTable_ ("constant/mu"),   
    kappaTable_ ("constant/kappa"),
    sigmaETable_ ("constant/sigmaE")
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Thermo>
void Foam::tabulatedTransport<Thermo>::tabulatedTransport::write(Ostream& os) const
{
    os  << this->name() << endl;
    os  << token::BEGIN_BLOCK  << incrIndent << nl;

    Thermo::write(os);

    dictionary dict("transport");
    os  << indent << dict.dictName() << dict;

    os  << decrIndent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Thermo>
Foam::Ostream& Foam::operator<<(Ostream& os, const tabulatedTransport<Thermo>& ct)
{
    operator<<(os, static_cast<const Thermo&>(ct));
	os << tab << ct.muTable_ << tab << ct.kappaTable_ << tab << ct.sigmaETable_;

    os.check("Ostream& operator<<(Ostream&, const tabulatedTransport&)");

    return os;
}


// ************************************************************************* //
