#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Punch_Left
		Type	Local
		At	LArmL
		Sound PPD_ForceMaul_Attack_01 100 100 1
		ChildFX	ENEMYFX\V_PraetorianPolice\ForceGlove\Child_ForceGlove_Attack_Left.fx
		Lifespan 25
	End

	Event
		EName	Punch_Right
		Type	Local
		At	LArmR
		ChildFX	ENEMYFX\V_PraetorianPolice\ForceGlove\Child_ForceGlove_Attack_Right.fx
		Lifespan 25
	End
End


#Condition
#	On 	Time
#	Time 	10
#
#	Event
#		EName 	hand2
#		Type	Local
#		At	WepL
#		Part1	POWERS\PowerPunch\Largepowerpunch1.part
#		Part2	POWERS\PowerPunch\Largepowerpunch2.part
#		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
#		Sound	PunchA 100 30 0.6
#	End
#
#	Event
#		EName 	hand
#		Type	Local
#		At	WepL
#		Part1	POWERS\PowerPunch\Largepowerpunch3.part
#		Part3   POWERS\PowerPunch\LargepowerpunchRings.part
#	End
#
#	Event
#		EName 	hand2
#		Type	Local
#		At	WepR
#		Part1	POWERS\PowerPunch\Largepowerpunch1.part
#		Part2	POWERS\PowerPunch\Largepowerpunch2.part
#		Part3   POWERS\PowerPunch\LargepowerpunchGlow.part
#	End
#
#	Event
#		EName 	hand
#		Type	Local
#		At	WepR
#		Part1	POWERS\PowerPunch\Largepowerpunch3.part
#		Part3   POWERS\PowerPunch\LargepowerpunchRings.part
#	End
#End

#############################################################

End