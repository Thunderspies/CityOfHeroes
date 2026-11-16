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
		Anim	Rocksuit_AnimFX
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