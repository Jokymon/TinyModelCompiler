TinyModelCompiler
=================

This is an early stage attempt at creating a fully C++-based OPC-UA Model
Compiler. The objective is to create a tool that turns XML-files following
the ModelDesign.xsd (https://github.com/OPCFoundation/UA-ModelCompiler/blob/master/ModelCompiler/UA%20Model%20Design.xsd)
into NodeSet2 files (https://github.com/OPCFoundation/UA-Nodeset/blob/master/Schema/UANodeSet.xsd)

Building
--------

The project should handle all external dependencies automatically using CMakes
`ExternalProject_add` feature. Currently it depends on libxml2 and Niels Lohmanns
FIFO map.

In a build directory simply do

```
cmake -G<Your prefered generator> <path to TinyModelCompiler>
cmake --build .
```

Usage
-----

After building you will find two executables in the build folder,
TinyModelCompiler and generate_ua_nodeset2. The latter is only used in certain
cases when developing on the compiler. It is the generator for the checked in
opc_ua_nodeset2.[h|cpp] source files.

To turn your ModelDesign file into a NodeSet2 file you run

  ./TinyModelCompiler YourModelDesign.xml


Disclaimer
----------

Note that this tool is still in a very early alpha stage and many important
features are not yet supported. However, contributions and extensions are
welcome.
