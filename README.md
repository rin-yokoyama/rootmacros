# rootmacros
Personal ROOT macros for data analysis

# Installation
Copy rootmacros/ directory and add following two lines to your rootlogon.C.
```c++
{
  gROOT->SetMacroPath("/PATH_TO_YOUR_ROOTMACROS_DIRECTORY/rootmacros/");
  gInterpreter->AddIncludePath("/PATH_TO_YOUR_ROOTMACROS_DIRECTORY/rootmacros/");
}
```

Then you can loads macros by .L command

# Descriptions
## IsoAna
IsoAna class is a framework to analyze a Energy vs Timing histogram of gamma-rays for isomer decays or beta decays.
