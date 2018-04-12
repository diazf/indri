indri
=====

This is a clone of Indri 5.12 with minor customizations.  

## customizations
###Condensed List Relevance Models

Added a few lines of code to `IndriRunQuery` to support  [Condensed List Relevance Models](https://dl.acm.org/citation.cfm?id=2808194.2809491).  When executing `IndriRunQuery`, just include `-rerankSize=k` to have the RM rerank `k` from an initial query likelihood retrieval.

##Notes

To build on OSX, be should to set `CXXFLAGS=-fno-tree-vectorize`.  Thanks to [Luke Gallagher](https://github.com/lgrz).  

