#############################################################
## GraniteArmorTransformDeath.fx
#############################################################

FxInfo

Flags IsArmor InheritAnimScale

#############################################################

Condition
	On 	Time
	Time	0

	Event
		Ename	RootAnchor
		Type	Local
		At	Root
	#	Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			Alpha		1
		End

		Flags	AdoptParentEntity
	#	Lifespan 1
	End

	Event
		Ename	RockSuit
		Type	Local
		At	RootAnchor
		BhvrOverride
			TintGeom		1
		End
		Anim	Rocksuit_AnimFX_Lava
	End
End

#############################################################

#Condition
#	On 	DEATH
#
#	Event
#		Type	Destroy
#		EName	ALL
#	End
#End

#############################################################

End