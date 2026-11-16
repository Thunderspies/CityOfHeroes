#########################################################
##	
##

FxInfo
LifeSpan	250
##################################

Condition
	On 	cycle
	Time 	3
	chance	.95

	Event
		Type	local
		At	WepR
		BHVR	:BallTurentLightPosition.bhvr
		part	:BallTurrentBlinkingLight.part
		part	:BallTurrentLazer.part

		LifeSpan  2

	End


End

Condition
	On 	Time
	Time 	0

	Event
		EName 	tracer1
		Type	local
		At	WepR
		BHVR	:BallTurentBlastOrient.bhvr
		Geom	Fx_GunBurst
		#Part	:TurretBlast.part
		#Part	:TurretBlast2.part
		
		LifeSpan  2

	End
	
	Event
		Type	local
		At	WepR
		Part1	WEAPONFX/TorchflameA.part
		Part2	WEAPONFX/TorchflameB.part
		
		LifeSpan  1
	End

End

Condition
	On 	Time
	Time 	1

	Event
		EName 	tracer
		Type	local
		At	WepR
		
		Part	:TurrentMuzzflash.part
		Part	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		
		LifeSpan  2

	End
		
End

Condition
	On 	Time
	Time 	0


	Event
		EName 	tracer2
		Type	local
		At	WepR	
		Part	:Turrentsmoke.part
		sound	shotgun3 120.0 30.0 .9
		LifeSpan	20
	End

End


##########################################################


End				