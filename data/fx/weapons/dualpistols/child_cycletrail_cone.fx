#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Start
#		At	Origin
#		ChildFX	WEAPONS\DualPistols\Child_VaporTrail_Cone.fx
#		BhvrOverride
#					PyrRotate	0 90 0
#		End
#		LifeSpan 30
#	End
#
#	Event
#		Type	Start
#		At	Origin
#		ChildFX	WEAPONS\DualPistols\Child_VaporTrail_Cone_TracersOnly.fx  
#		BhvrOverride
#					PyrRotate	0 90 0
#		End
#		LifeSpan 30
#	End
#End

Condition
	On 	Cycle
	Time 	3

	Event
		Type	Start
		At	Origin
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail_Cone.fx
		LifeSpan 15
	End

	Event
		Type	Start
		At	Origin
		ChildFX	WEAPONS\DualPistols\Child_VaporTrail_Cone_TracersOnly.fx
		LifeSpan 30
	End
End

#############################################################

End