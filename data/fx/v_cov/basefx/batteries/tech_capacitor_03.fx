#########################################################
##	Tech Battery 03
########################################################
FxInfo

##########################  Small Arc #############################
Condition
	On 	Time
	Time 	2

	Event
		EName	Emitter05
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-0 -2.9 2.35
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