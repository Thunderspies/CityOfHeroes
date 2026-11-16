#########################################################
##	
##
FxInfo

##########################################################

Condition
		
	Event
		Ename	Top
		Type	start 
		At	root
		Bhvr	:spin.bhvr
		geom	TriageSpinnerTop
				
	End
		
	Event
		Ename	Base
		Type	start 
		At	root
		geom	TriageBase

	End
	
	Event
		Ename	Base
		Type	start 
		At	root
		BHVR	:BaseEnergyOffset.bhvr
		Part	:RedEnergy.part
		Part	:RedEnergyScreenFacing.part
	End

End

Condition
	On 	cycle
	Time 	20
	Chance	1

	Event
		Ename	Base
		Type	start 
		At	root
		BHVR	:DamageSplat.bhvr
		Splat	Generic_GlowSquare.tga		
	
		LifeSpan	30
	
	End

#	Event
#		Ename	Base
#		Type	start 
#		At	root
#		BHVR	:DamageSplat.bhvr
#		Splat	Generic_EnergySquare.tga		
#	
#		LifeSpan	40
#	
#	End
End


Condition
	On 	cycle
	Time 	20
	Chance	1

	Event
		Type	start 
		At	Base
		altpiv	1
		BHVR	:Lightscale.bhvr
		Part	:RedLightPulsing.part
		Part	:WhiteLightPulsing.part
		LifeSpan	10
				
	End

		
	Event
		Type	start 
		At	Base
		altpiv	2
		
		BHVR	:Lightscale2.bhvr
		Part	:RedLightPulsing.part
		Part	:WhiteLightPulsing.part
				
		LifeSpan	10
	End

		
	Event
		Type	start 
		At	Base
		altpiv	3
		
		BHVR	:Lightscale3.bhvr
		Part	:RedLightPulsing.part
		Part	:WhiteLightPulsing.part
				
		LifeSpan	10
	End
	
		
	Event
		Type	start 
		At	Base
		altpiv	4
		
		BHVR	:Lightscale4.bhvr
		Part	:RedLightPulsing.part
		Part	:WhiteLightPulsing.part
			
		LifeSpan	10	
	End

End


End			