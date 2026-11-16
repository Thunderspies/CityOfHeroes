#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo
Lifespan 300

#############################################################
## LOOP
#############################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local
		At	Chest
		Part	:Decomp_HitSplatter_Mult_Small.part
		#Part	:Decomp_HitSplatter_Mult2.part
		Part	:Decomp_Splatter_Spray_Small.part
		PArt	:Decomp_HitFlash_Small.part
		Lifespan 5
	End

End


#############################################################

End