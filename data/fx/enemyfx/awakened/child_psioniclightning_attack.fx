#############################################################
## PsionicShockwave_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 50.0 0.0
		End
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Hand_Bolt_Blur_Small.part
		LookAt	WepR
		Pother	WepR
		Lifespan 1
	End
End


#############################################################

End