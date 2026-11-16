#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part	:Decomp_HitSplatter_Mult.part
		Part	:Decomp_HitSplatter_Mult2.part
		Part	:Decomp_Splatter_Spray.part
		Lifespan 5
	End

End


#############################################################

End