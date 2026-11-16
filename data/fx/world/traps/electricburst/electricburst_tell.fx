#############################################################
## ElectricBurst_Tell.fx
#############################################################

FxInfo
LifeSpan 500
#############################################################
#######################	ONE

Condition
	On 	Time
	Time 	0

	Event
		Ename	ArcCenter
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 3 0
		End
	End

	Event
		Type	start
		At	ArcCenter
		Part	:ElectricBurst_Arcs03.part
		Part	:ElectricBurst_GlowFlash2.part
		Sound 	Sewer_ElectricBurstMediumHigh_Loop 100 100 .8
	End
End
#############################################################
End