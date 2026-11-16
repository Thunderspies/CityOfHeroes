#############################################################
## MediumFireSlow.fx
#############################################################

FxInfo
LifeSpan 0
Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		ChildFX	:Meteor_Fire_Child.fx
	End

	Event
		Type	Local
		At	Root
		Sound 	Fire_Small02_Loop 50 50 .3
	End

End

Condition
	On 	Time
	Time 	33

	Event
		Type	Local
		At	Root
		ChildFX	:Meteor_Fire_Child.fx
	End
End

Condition
	On 	Time
	Time 	41

	Event
		Type	Local
		At	Root
		ChildFX	:Meteor_Fire_Child.fx
	End
End

Condition
	On 	Time
	Time 	67

	Event
		Type	Local
		At	Root
		ChildFX	:Meteor_Fire_Child.fx
	End
End

Condition
	On 	Time
	Time 	93

	Event
		Type	Local
		At	Root
		ChildFX	:Meteor_Fire_Child.fx
	End
End

Condition
	On 	Time
	Time 	113

	Event
		Type	Local
		At	Root
		ChildFX	:Meteor_Fire_Child.fx
	End
End

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Prime
#		Type	Local
#		At	Root
#		BhvrOverride
#			PositionOffset 	0 -1 0
#			#PyrRotate	-30 0 0
#		End
#		Part	AAA_TEST\FIRE\Fire_Combustible_Add.part
#		Part	AAA_TEST\FIRE\Fire_Combustible_Alpha.part
#	End
#End

#############################################################

End