#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		Type	Local
		At	LArmR
		Bhvr	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_AttackFade.bhvr
		BhvrOverride
			StartColor	255 255 128
		End
		Part	ENEMYFX\V_PraetorianPolice\ForceGlove\ForceGlove_Attack_Glow01.part
		Geom	GEO_FormShield_Gauntlet_LArmR
		pOther	WepR
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