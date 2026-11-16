#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event

		Type	Local
		At	Root
		BhvrOverride
			PYRRotateJitter	5 5 5
		End
		Part	:Heal_Swirl_Flat_Smokey.part
		Lifespan	15
	End

End


#############################################################

End