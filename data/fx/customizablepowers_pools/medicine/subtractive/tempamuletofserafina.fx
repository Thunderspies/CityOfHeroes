#########################################################
##	chill touch hit
FxInfo

Input
	InpName	wepL
End

Lifespan 120

##########################################
##  AUDIO
##########################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	hips
		Sound regen5b 70.0 70.0 .6
	End
End


Condition
	On	Time
	Time	50

	Event
		Type	Local
		At	hips
		Sound coralamulet 70.0 70.0 .5
	End
End

######################################################################

Condition
	On 	Time
	Time 	5


	Event
		EName	HookUp
		Type	Local
		At	Root
		Geom	RootToHeadAdjustment

	End

	Event
		EName	RingsDown
		Type	Local
		At	HookUp
		Altpiv	1

		Part1	Powers/Psionics/PsionicRingsDown.part
		Part1	Powers/Psionics/PsionicRingsDown2.part

		Lifespan 70
	End

End

Condition
	On 	Time
	Time 	27

	Event
		EName	RingsOut
		Type	Local
		At	Root

		Part1	Powers/Psionics/PsionicRingsOut.part
		Part1	Powers/Psionics/PsionicRingsOut2.part

		Lifespan 60
	End

End

End