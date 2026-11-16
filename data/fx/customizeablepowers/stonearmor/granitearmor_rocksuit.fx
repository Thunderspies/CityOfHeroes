#############################################################
## GraniteArmorTransformDeath.fx
#############################################################

FxInfo

Flags IsArmor

#############################################################

Condition
	On 	Time
	Time	0

	Event
		Ename	RockSuit
		Type	Local
		At	Root
		Flags	AdoptParentEntity
	#	Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			Alpha		1
		End
	End

	Event
		Type	Local
		At	RockSuit
		BhvrOverride
			Alpha		255
			TintGeom	1
			Scale		2.0 2.0 2.0
		End
		#Anim	Rocksuit_AnimFX_ColorTintable
		Anim	Rocksuit_AnimFX_Crystal
		#Anim	Rocksuit_AnimFX_Lava
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