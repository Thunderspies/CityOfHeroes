#########################################################
##	template

FxInfo

LifeSpan       40

Condition

	On 	Time	
	Time	0

	Event	
		Type	Local
		At	chest
		Sound thunder3 100 100 .75
	End

End

Condition

	On 	Time	
	Time	5

	Event	

		Type	Local
		At	chest
		Sound thunderclap1 100 100 1.0
	End

End

Condition

	On 	Time	
	Time	0

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Overhead
		AltPiv	1
		part	:ClapHandGlow.part
		part	:ClapHandLightning.part
		part	:ClapHandLightningDark.part
		part	:ClapHandLightningReal.part
		
		LifeSpan 10	
	End

End

Condition

	On 	Time	
	Time	5

	Event	
		ENAME	Overhead
		Type	Local
		At	Root
		Part	:Sparklies.part
		Geom	OverHeadDummy
		LifeSpan 15	
	End

	Event	
		ENAME	Flash
		Type	Local
		At	Overhead
		AltPiv	1
		part	:ClapFlash.part
		Bhvr	Behaviors/SSThunderClapShake.bhvr
		LifeSpan 17	
	End

End

Condition

	On 	Time	
	Time	10

	Event	
		
		Type	Local
		At	Root
		Part	:Sparklies.part
		Part	:BlueSparklies.part
		
		Lifespan	25
	End



End



	