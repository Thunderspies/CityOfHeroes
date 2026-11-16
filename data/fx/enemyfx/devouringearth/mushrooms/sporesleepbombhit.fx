#########################################################
##	
##

FxInfo
LifeSpan  200


Input  
	InpName	Origin
End

Input  
	InpName	T_Chest
End

Input  
	InpName	Root
End

##################################
Condition

	On 	time
	Time 	0
	
	Event
		EName   RadiusFxScale2
		Type	local
		At	Root
		GEom	RootToChestAdjustment
		Sound mushroombombhit 60 60 .8
		LifeSpan	30
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	RadiusFxScale2
		altpiv	1
		Bhvr	Behaviors/Spores.bhvr
		LifeSpan	30
	End

	Event
		EName 	Prime
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/SporeRingScale.bhvr
		Part1	:SporeCloud01x.part
		Part2	:SporeCloud02x.part
		
		Part4	:SporeGlowsx.part
		
		LifeSpan	35
	End		

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    behaviors/SporeRingScaleSlower.bhvr
	
		Part3	:SporeDustx.part
		Part	:SporeDustFlatx.part
		
		LifeSpan	35
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
			
		Part3	:SporeExplosion.part
		Part	:SporeExplosionMist.part
		Part	:SporeGlowsScatter.part
		
		LifeSpan	17
	End

End

End				