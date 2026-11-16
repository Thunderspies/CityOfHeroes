#########################################################
##	
##
FxInfo

##########################################################

Condition
	Event
		Ename	hookup
		Type	Local 
		At		root
		Bhvr	:TriangleSplatOffset.bhvr		
	End
		
	Event
		Ename	Top
		Type	Local 
		At		hookup
		Bhvr	:trispin.bhvr
		geom	TriageHealSpinnerTop02
				
	End
		
	Event
		Ename	Base
		Type	Local 
		At		hookup
		BhvrOverride
			PyrRotate 0 1.35 0
		End
		geom	TriageBaseHeal
	End
	
	Event
		Ename	Base
		Type	Local 
		At		hookup
		BHVR	:TriBaseEnergyOffset.bhvr
		Part	:GreenEnergy.part
		Part	:GreenEnergyScreenFacing.part
	End

	Event
		Type	start 
		At		hookup
		Splat	Generic_GlowTriangle
		Bhvr	:HealingGlowSplat.bhvr
		LifeSpan	80
	End
End

Condition
	On 	cycle
	Time 	60

	Event
		Type	start 
		At		hookup
		Splat	Generic_GlowTriangle
		Bhvr	:HealingGlowSplat.bhvr
		LifeSpan	80
	End
End


End			