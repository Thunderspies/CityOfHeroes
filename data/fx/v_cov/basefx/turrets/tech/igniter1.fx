#########################################################
##	
##

FxInfo
LifeSpan  72

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
#		Anim	FX_turret_flamethrower_evil
#		LifeSpan	114
#		
#	End
#
#End

#################################


Condition
	On	Time
	Time	0

	Event
		Type	local
		At	RingL
		Sound servoflamethrower 100 100 .7
	End
End


Condition
	On 	Time
	Time 	20


	Event
		ENAME   PivotOrientAdjuster
		Type	Local 
		At	wepR
		Bhvr	:rot1.bhvr
	End

	Event
		EName 	Prime
		Type	start 
		At	PivotOrientAdjuster
		bhvr	:ignitehit.bhvr
#		geom	testing_targetb
		Magnet	Target
		LookAt	Target
		
	End
	Event
		ENAME   PivotOrientAdjusterL
		Type	Local 
		At	wepL
		Bhvr	:rot1.bhvr
	End

	Event
		EName 	Blast1
		Type	local
		At	PivotOrientAdjuster
		
		
		#Sound	flamethrower2 120.0 30.0 .95
		
		Part1	WEAPONFX\FlameThrowerFlame.part		
		Part	WEAPONFX\FireSpecksSpit.part
		Part	WEAPONFX\FireSpecksFalling.part
		Part	WEAPONFX\FireSpit2.part
		Part	WEAPONFX\FireSpitStart.part
	End

	Event
		EName 	Blast1L
		Type	local
		At	PivotOrientAdjusterL
		
		
		#Sound	flamethrower2 120.0 30.0 .95
		
		Part1	WEAPONFX\FlameThrowerFlame.part		
		Part	WEAPONFX\FireSpecksSpit.part
		Part	WEAPONFX\FireSpecksFalling.part
		Part	WEAPONFX\FireSpit2.part
		Part	WEAPONFX\FireSpitStart.part
	End

	Event
		EName 	Blast2
		Type	local
		At	PivotOrientAdjuster
		
		Part1	WEAPONFX/TorchflamethrowerA.part
		Part2	WEAPONFX\TorchflamethrowerB.part
		
	End

	Event
		EName 	Blast2L
		Type	local
		At	PivotOrientAdjusterL
		
		Part1	WEAPONFX/TorchflamethrowerA.part
		Part2	WEAPONFX\TorchflamethrowerB.part
		
	End

End

#Condition
#	On 	Time
#	Time 	65
#
#	Event
#		EName 	Blast2
#		Type	Destroy
#				
#	End
#End
#
#Condition
#	On 	Time
#	Time 	79
#
#	Event
#		EName 	Blast3
#		Type	Destroy
#				
#	End
#End
#
#Condition
#	On 	Time
#	Time 	85
#
#	Event
#		EName 	Blast1
#		Type	Destroy
#				
#	End
#End
#
#Condition
#	On 	Time
#	Time 	70
#
#	Event
#		EName 	tracer
#		Type	Destroy
#				
#	End
#
#End

End