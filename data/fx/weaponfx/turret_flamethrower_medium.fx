#########################################################
##	
##

FxInfo
LifeSpan  200

Input  
	InpName	Target
End

Input  
	InpName	RingL
End

Input  
	InpName	WepR
End

##################################

## this would have been the actual flamethrower geometry, but it is now move to the enttype file

#Condition
#	On 	Time
#	Time 	1
#
#	Event
#		EName 	Flamethrower
#		Type	Local 
#		At	T1_L
#		Anim	FX_turret_flamethrower
#		LifeSpan	114
#		
#	End
#
#End

#################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	start 
		At	RingL
	
		Magnet	Target
		LookAt	Target
		
	End

	Event
		ENAME   PivotOrientAdjuster
		Type	Local 
		At	RingL
		Bhvr	Behaviors/turn_90.bhvr
	End

	Event
		EName 	Blast1
		Type	local
		At	PivotOrientAdjuster
		
		
		Sound flamethrower2 120.0 120.0 .95
		
		Part1	:FlameThrowerFlame.part		
		Part	:FireSpecksSpit.part
		Part	:FireSpecksFalling.part
		Part	:FireSpit2.part
		Part	:FireSpitStart.part
	End

	Event
		EName 	Blast2
		Type	local
		At	PivotOrientAdjuster
		
		Part1	WEAPONFX/TorchflamethrowerA.part
		
	End

	Event
		EName 	Blast3
		Type	local
		At	PivotOrientAdjuster
		
		
		Part2	:TorchflamethrowerB.part
	End

End

Condition
	On 	Time
	Time 	65

	Event
		EName 	Blast2
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	79

	Event
		EName 	Blast3
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	85

	Event
		EName 	Blast1
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	70

	Event
		EName 	tracer
		Type	Destroy
				
	End

End

End