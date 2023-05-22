
// https://alx71hub.github.io/hcb/ with modifications to remove right recursion
// `opt` specifies that the production is optional (left associative)
// Productions start with an uppercase letter, terminals are either symbols or start with lowercase letters (keywords)

Program := DeclarationSeq $

DeclarationSeq := DeclarationSeq Declaration
DeclarationSeq := Declaration

{ // block declaration
  Declaration := BlockDeclaration

  BlockDeclaration :=
    SimpleDeclaration
    AsmDefinition
    NamespaceAliasDefinition
    UsingDeclaration
    UsingDirective
    StaticAssertDeclaration
    AliasDeclaration
    OpaqueEnumDeclaration
  
  { // simple declaration
    SimpleDeclaration := AttributeSpecifierSeq `opt` DeclSpecifierSeq `opt` InitDeclaratorSeq `opt` ;
  }
}

{ // function definition
  Declaration := FunctionDefinition

  FunctionDefinition :=
    AttributeSpecifierSeq `opt` DeclSpecifierSeq `opt` Declarator FunctionBody
    AttributeSpecifierSeq `opt` DeclSpecifierSeq `opt` Declarator = default ;
    AttributeSpecifierSeq `opt` DeclSpecifierSeq `opt` Declarator = delete ;

  FunctionBody :=
    CtorInitializer `opt` CompoundStatement
    FunctionTryBlock
}

{ // template declaration
  Declaration := TemplateDeclaration

  TemplateDeclaration := template < TemplateParameterList > Declaration

  TemplateParameterList :=
    TemplateParameter
    TemplateParameterList , TemplateParameter
  
  { // template parameter
    TemplateParameter :=
      TypeParameter
      ParameterDeclaration

    TypeParameter :=
      class ... `opt` Identifier `opt`
      class Identifier `opt` = TypeId
      typename ... `opt` Identifier `opt`
      typename Identifier `opt` = TypeId
      template < TemplateParameterList > class ... `opt` Identifier `opt`
      template < TemplateParameterList > class Identifier `opt` = IdExpression
  }
}

{ // explicit instantiation
  Declaration := ExplicitInstantiation

  ExplicitInstantiation := extern `opt` template Declaration
}

{ // explicit specialization
  Declaration := ExplicitSpecialization

  ExplicitSpecialization :=	template < > Declaration
}

{ // linkage specification
  Declaration := LinkageSpecification

  LinkageSpecification :=	 	 
    extern StringLiteral { DeclarationSeq `opt` }
    extern StringLiteral Declaration
}

{ // namespace definition
  Declaration := NamespaceDefinition

  NamespaceDefinition :=	 	 
    NamedNamespaceDefinition
    UnnamedNamespaceDefinition

  NamedNamespaceDefinition :=
    OriginalNamespaceDefinition
    ExtensionNamespaceDefinition

  OriginalNamespaceDefinition :=
    inline `opt` namespace Identifier { NamespaceBody }

  ExtensionNamespaceDefinition :=
    inline `opt` namespace OriginalNamespaceName { NamespaceBody }

  UnnamedNamespaceDefinition :=
    inline `opt` namespace { NamespaceBody }

  NamespaceBody :=
    DeclarationSeq `opt`
}

{ // empty declaration
  Declaration := EmptyDeclaration

  EmptyDeclaration := ;
}

{ // attribute declaration
  Declaration := AttributeDeclaration

  AttributeDeclaration := AttributeSpecifierSeq ;
}

{ // attribute specifier seq
  AttributeSpecifierSeq :=
    AttributeSpecifier
    AttributeSpecifierSeq AttributeSpecifier

  AttributeSpecifier :=
    [ [ AttributeList ] ]
    AlignmentSpecifier

  AlignmentSpecifier :=
    alignas ( TypeId ... `opt` )
    alignas ( AssignmentExpression ...`opt` )

  AttributeList :=
    Attribute `opt`
    AttributeList , Attribute `opt`
    Attribute ...
    AttributeList , Attribute ...

  Attribute :=
    AttributeToken AttributeArgumentClause `opt`

  AttributeToken :=
    Identifier
    AttributeScopedToken

  AttributeScopedToken :=
    AttributeNamespace :: Identifier

  AttributeNamespace :=
    Identifier

  AttributeArgumentClause :=
    ( BalancedTokenSeq )

  BalancedTokenSeq :=
    BalancedToken
    BalancedTokenSeq BalancedToken

  BalancedToken :=
    ( BalancedTokenSeq )
    [ BalancedTokenSeq ]
    { BalancedTokenSeq }
    Token  // except a parenthesis, a bracket, or a brace
}

{ // decl specifier seq
  DeclSpecifierSeq :=
   	AttributeSpecifierSeq `opt` DeclSpecifier  
    DeclSpecifierSeq DeclSpecifier

  DeclSpecifier :=
    StorageClassSpecifier
    TypeSpecifier
    FunctionSpecifier
    friend
    typedef
    constexpr

}

{ // init declarator seq
  InitDeclaratorSeq :=
    InitDeclarator
    InitDeclaratorSeq , InitDeclarator

  InitDeclarator :=
    Declarator Initializer `opt`

  Declarator :=
    PtrDeclarator
    NoptrDeclarator ParametersAndQualifiers TrailingReturnType

  PtrDeclarator :=
    NoptrDeclarator
    PtrDeclarator PtrOperator

  NoptrDeclarator :=
    DeclaratorId AttributeSpecifierSeq `opt`
    NoptrDeclarator ParametersAndQualifiers
    NoptrDeclarator [ ConstantExpression `opt` ] AttributeSpecifierSeq `opt`
    ( PtrDeclarator )

  ParametersAndQualifiers :=
    ( ParameterDeclarationClause ) AttributeSpecifierSeq `opt` CvQualifierSeq `opt` RefQualifier `opt` ExceptionSpecification `opt`

  TrailingReturnType :=
    -> TrailingTypeSpecifierSeq AbstractDeclarator `opt`

  PtrOperator :=
    * AttributeSpecifierSeq `opt` CvQualifierSeq `opt`
    & AttributeSpecifierSeq `opt`
    && AttributeSpecifierSeq `opt`
    :: `opt` NestedNameSpecifier * AttributeSpecifierSeq `opt` CvQualifierSeq `opt`

  CvQualifierSeq :=
    CvQualifier
    CvQualifierSeq CvQualifier

  CvQualifier :=
    const
    volatile

  RefQualifier :=
    &
    &&

  DeclaratorId :=
    ... `opt` IdExpression
    :: `opt` NestedNameSpecifier `opt` ClassName
}
