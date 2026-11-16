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
		ename base
		Type	local
		At	root
		bhvroverride
			PositionOffset		0 6.7 0
		end
		Part1	POWERS\FireControl\FireBallTargetSteam.part
		Part2	POWERS\FireControl\FireBallTargetFlame2.part
		Part3	POWERS\FireControl\FireBallSpark.part	
		Sound torch2_loop 55.0 55.0 .5
#		childfx :Arcane_Mockup1.fx
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	start
		At	base
		Sound flamethrower 100 100 .7
	End
End


Condition
	On 	Time
	Time 	20

	Event
		ENAME   looker
		Type	start
		At	base
#		bhvroverride
#			PositionOffset		6 6.5 0
#		end
		lookat	Target
	End

	Event
		ENAME   PivotOrientAdjuster
		Type	local
		At	looker
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
		EName 	Blast2
		Type	local
		At	PivotOrientAdjuster
		
		Part1	WEAPONFX/TorchflamethrowerA.part
		Part2	WEAPONFX\TorchflamethrowerB.part
		
	End

End

End