#########################################################
##	
##

FxInfo


LifeSpan	60
##################################
##################################
##################################

Condition
	On 	Time
	Time 	5



	Event
		
		Type	Local
		At	chest
		Sound Darthit2 70 70 .9
	End
End

##################################
##################################
##################################

Condition
	On 	Time
	Time 	0

	Event	
		ENAME	Flasher
		Type	Start
		At	Chest	
		Part	:silverSparks1.part
		Part4	:SilverHitStar01.part
		Part4	:SilverHitGlow01.part
		Part5	:SilverHitRays01.part
		Lifespan 2
		
	End

	Event

		ENAME	Spikey
		Type	Local
		At	chest
		Bhvr	:impactSilverDart.bhvr
		Geom	FX_SilverSpike01
		
	End
	
End



End				