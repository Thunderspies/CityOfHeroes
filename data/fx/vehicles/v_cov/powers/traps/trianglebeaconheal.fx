#########################################################
##	
##
FxInfo

##########################################################

Condition
		
	Event
		Ename	Top
		Type	Local 
		At	root
		Bhvr	:trispin.bhvr
		geom	TriageHealSpinnerTop02
				
	End
		
	Event
		Ename	Base
		Type	Local 
		At	root
		geom	TriageBaseHeal

	End
	
	Event
		Ename	Base
		Type	Local 
		At	root
		BHVR	:TriBaseEnergyOffset.bhvr
		Part	:GreenEnergy.part
		Part	:GreenEnergyScreenFacing.part
	End

	Event
		Ename	SplatHookup
		Type	Local 
		At	root
		BHVR	:TriangleSplatOffset.bhvr
		
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	1


	Event
		Ename	Base
		Type	Local 
		At	root
		BHVR	:HealingGlowSplat.bhvr
		#Splat	Generic_GlowSquare.tga		
		Splat	Generic_GlowTriangle.tga
	
		LifeSpan	35
	End

	Event
		Ename	Base
		Type	Local 
		At	root
		BHVR	:HealingSplat.bhvr
		#Splat	Generic_EnergySquare.tga
		Splat	Generic_EnergyTriangle.tga		
	
		LifeSpan	35
	
	End
End


#Condition
#	On 	cycle
#	Time 	20
#	Chance	1
#
#	Event
#		Type	Local 
#		At	Base
#		altpiv	1
#		BHVR	:Lightscalea.bhvr
#		Part	:RedLightPulsing.part
#		Part	:WhiteLightPulsing.part
#		LifeSpan	10
#				
#	End
#
#		
#	Event
#		Type	Local 
#		At	Base
#		altpiv	2
#		
#		BHVR	:Lightscale2a.bhvr
#		Part	:RedLightPulsing.part
#		Part	:WhiteLightPulsing.part
#				
#		LifeSpan	10
#	End
#
#		
#	Event
#		Type	Local 
#		At	Base
#		altpiv	3
#		
#		BHVR	:Lightscale3a.bhvr
#		Part	:RedLightPulsing.part
#		Part	:WhiteLightPulsing.part
#				
#		LifeSpan	10
#	End
#	
#End


End			