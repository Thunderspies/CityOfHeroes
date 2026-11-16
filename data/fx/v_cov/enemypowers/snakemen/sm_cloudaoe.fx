#########################################################
##	Inferno FX
##
FxInfo

LifeSpan	40

Condition
	On Time
	Time 0

	Event
		Type	Local
		At	hips
		Sound mushroomburst2 80 80 .99
	End
End


#####################################################################################

Condition

	On 	Time
	Time 	0 
	
	Event
		EName   RadiusFxScale
		Type	local
		At	Root

		Bhvr	Behaviors/Spores.bhvr
		LifeSpan	30
	End

	Event
		EName 	Prime
		Type	Local 
		At	RadiusFxScale

		bhvr    behaviors/FX_SM_SprayRingScale.bhvr
		Part1	:SM_SprayCloud01.part
		Part2	:SM_SprayCloud02.part
#		Part4	:SM_SprayGlows.part
		
		LifeSpan	35
	End
	
	Event
		EName 	Pool
		Type	Local 
		At	Root

		bhvr    behaviors/FX_SM_SprayPoolScale.bhvr
		Part1	:SM_SprayCloudStretch02.part
		
		LifeSpan	25
	End

#	Event
#		EName 	Prime1
#		Type	Local 
#		At	RadiusFxScale
#		
#		bhvr    behaviors/SporeRingScaleSlower.bhvr
#		Part3	:SM_SprayDust.part
#		Part	:SM_SprayDustFlat.part
#		
#		LifeSpan	35
#	End

End


End
