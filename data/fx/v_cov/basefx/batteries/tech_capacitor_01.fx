#########################################################
##	Tech Battery 01
########################################################
FxInfo

##########################  Large Arc #############################
Condition
	On 	Time
	Time 	0

	Event
		EName	Emitter01
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-7.5 -5 2.25
		End
		Part	:Capacitor_Glow.part
						
	End

	Event
		EName	Lightning01
		Type	Local
		At	Emitter01
		BhvrOverride
			PositionOffset	0 9.7 0
		End
		Pmagnet	Emitter01
		Part	:Capacitor_Glow.part
		Part	:Capacitor_LightningArc.part
						
	End
End
###############################################################

##########################  Large Arc #############################
Condition
	On 	Time
	Time 	6

	Event
		EName	Emitter02
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-3.5 -5 2.25
		End
		Part	:Capacitor_Glow.part
						
	End

	Event
		EName	Lightning02
		Type	Local
		At	Emitter02
		BhvrOverride
			PositionOffset	0 9.7 0
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
			PositionOffset	0.0 -5 2.25
		End
		Part	:Capacitor_Glow.part
						
	End

	Event
		EName	Lightning03
		Type	Local
		At	Emitter03
		BhvrOverride
			PositionOffset	0 9.7 0
		End
		Pmagnet	Emitter03
		Part	:Capacitor_Glow.part
		Part	:Capacitor_LightningArc.part
						
	End
End
###############################################################

##########################  Large Arc #############################
Condition
	On 	Time
	Time 	4

	Event
		EName	Emitter04
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	4 -5 2.25
		End
		Part	:Capacitor_Glow.part
						
	End

	Event
		EName	Lightning04
		Type	Local
		At	Emitter04
		BhvrOverride
			PositionOffset	0 9.7 0
		End
		Pmagnet	Emitter04
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
			PositionOffset	-5.5 -2.1 2.5
		End
		Part	:Capacitor_Glow.part
						
	End

	Event
		EName	Lightning05
		Type	Local
		At	Emitter05
		BhvrOverride
			PositionOffset	0 5.8 0
		End
		Pmagnet	Emitter05
		Part	:Capacitor_Glow.part
		Part	:Capacitor_LightningArc.part
						
	End
End
###############################################################

##########################  Small Arc #############################
Condition
	On 	Time
	Time 	5

	Event
		EName	Emitter06
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	1.9 -2.1 2.5
		End
		Part	:Capacitor_Glow.part
						
	End

	Event
		EName	Lightning06
		Type	Local
		At	Emitter06
		BhvrOverride
			PositionOffset	0 5.8 0
		End
		Pmagnet	Emitter06
		Part	:Capacitor_Glow.part
		Part	:Capacitor_LightningArc.part
						
	End
End
###############################################################


End