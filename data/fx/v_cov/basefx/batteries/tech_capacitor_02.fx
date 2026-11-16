#########################################################
##	Tech Battery 02
########################################################
FxInfo

##########################  Large Arc #############################
Condition
	On 	Time
	Time 	6

	Event
		EName	Emitter02
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-4 -4.9 2.25
		End
		Part	:Capacitor_Glow.part
						
	End

	Event
		EName	Lightning02
		Type	Local
		At	Emitter02
		BhvrOverride
			PositionOffset	0 9.5 0
		End
		Pmagnet	Emitter02
		Part	:Capacitor_Glow.part
		Part	:Capacitor_LightningArc.part
						
	End
End
###############################################################

##########################  Large Arc #############################
Condition
	On 	Time
	Time 	1

	Event
		EName	Emitter03
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -4.9 2.25
		End
		Part	:Capacitor_Glow.part
						
	End

	Event
		EName	Lightning03
		Type	Local
		At	Emitter03
		BhvrOverride
			PositionOffset	0 9.5 0
		End
		Pmagnet	Emitter03
		Part	:Capacitor_Glow.part
		Part	:Capacitor_LightningArc.part
						
	End
End
###############################################################


##########################  Small Arc #############################
Condition
	On 	Time
	Time 	2

	Event
		EName	Emitter05
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-2 -2.9 2.35
		End
		Part	:Capacitor_Glow.part
						
	End

	Event
		EName	Lightning05
		Type	Local
		At	Emitter05
		BhvrOverride
			PositionOffset	0 5.5 0
		End
		Pmagnet	Emitter05
		Part	:Capacitor_Glow.part
		Part	:Capacitor_LightningArc.part
						
	End
End
###############################################################


End