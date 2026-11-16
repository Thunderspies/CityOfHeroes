#############################################################
## Child_ElectricalArc.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Cycle
	Time	7
	Chance	0.5

	Event
		Type	Local
		At	Origin
		Part	:ElectricityArchContinuingz_Lrg.part
		Part	:ElectricityArchContinuing2z_Lrg.part
		Part	:ElectricityArchContinuingzWhite_Lrg.part
		Part	:ElectricityArchContinuing2zWhite_Lrg.part
		Part	:ElectricitySparkBigGlow_Lrg.part
		Part	:ElectricitySparkStar_Lrg.part
		Part	:ElectricitySparkRing_Lrg.part
		Lifespan 7
	End
End

#############################################################

End