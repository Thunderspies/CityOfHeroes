#########################################################
##	
##

FxInfo

Lifespan	210

##################################
Condition
	On 	Time
	Time 	20

	Event
		EName 	tracer
		Type	Start
		At	WepL
		
		Geom	Fx_GunBurst
		Bhvr	:TurentBlastOrient.bhvr
		
		LifeSpan  2

	End


	Event
		EName 	Prime
		Type	start
		At	WepL
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missileFrag
		Part1	:Missile_smoke.part
		Part2	:Missile_smoke2.part
		Part	:Missile_fire.part
		Part	:Missile_fire2.part
	
		Magnet	T_Chest
		LookAt	T_Chest
	End

	
	Event
		EName 	Blast
		Type	start
		At	WepL		
		Bhvr	:TurentBlastOrient1.bhvr
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	:MissleSmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
		LifeSpan	8
			
	End

	Event
		EName 	KickBackSmoke
		Type	local
		At	WepL
		Bhvr	:TurentBlastOrientEndSmoke.bhvr
		Part1	:TurrentEndSmoke.part
		Part2	:MissleSmoke.part
		#Part4	WEAPONFX/Gunfire_MissleShell.Part
		LifeSpan	8
	End

End

#######################################################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	tracer1
		Type	Start
		At	WepR
		
		Geom	Fx_GunBurst
		Bhvr	:TurentBlastOrient.bhvr
		
		LifeSpan  2

	End


	Event
		EName 	Prime1
		Type	start
		At	WepR
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missileFrag
		Part1	:Missile_smoke.part
		Part2	:Missile_smoke2.part
		Part	:Missile_fire.part
		Part	:Missile_fire2.part
	
		Magnet	T_Chest
		LookAt	T_Chest
	End

	
	Event
		EName 	Blast1
		Type	start
		At	WepR		
		Bhvr	:TurentBlastOrient1.bhvr
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	:MissleSmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
		LifeSpan	8

	End

	Event
		EName 	KickBackSmoke1
		Type	local
		At	WepR
		Bhvr	:TurentBlastOrientEndSmoke.bhvr
		Part1	:TurrentEndSmoke.part
		Part2	:MissleSmoke.part
		#Part4	WEAPONFX/Gunfire_MissleShell.Part
		LifeSpan	8
	End

	
End


#######################################################################################

Condition
	On 	Time
	Time 	50

	Event
		EName 	tracer
		Type	Start
		At	WepL
		
		Geom	Fx_GunBurst
		Bhvr	:TurentBlastOrient.bhvr
		
		LifeSpan  2

	End


	Event
		EName 	Prime2
		Type	start
		At	WepL
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missileFrag
		Part1	:Missile_smoke.part
		Part2	:Missile_smoke2.part
		Part	:Missile_fire.part
		Part	:Missile_fire2.part
	
		Magnet	T_Chest
		LookAt	T_Chest
	End

	
	Event
		EName 	Blast
		Type	start
		At	WepL		
		Bhvr	:TurentBlastOrient.bhvr
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	:MissleSmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
		LifeSpan	8
			
	End

	Event
		EName 	KickBackSmoke
		Type	local
		At	WepL
		Bhvr	:TurentBlastOrientEndSmoke.bhvr
		Part1	:TurrentEndSmoke.part
		Part2	:MissleSmoke.part
		#Part4	WEAPONFX/Gunfire_MissleShell.Part
		LifeSpan	8
	End

End

#######################################################################################
###################################################################################

Condition
	On 	Time
	Time 	65

	Event
		EName 	tracer1
		Type	Start
		At	WepR
		
		Geom	Fx_GunBurst
		Bhvr	:TurentBlastOrient.bhvr
		
		LifeSpan  2

	End


	Event
		EName 	Prime3
		Type	start
		At	WepR
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missileFrag
		Part	:Missile_smoke.part
		Part	:Missile_smoke2.part
		Part	:Missile_fire.part
		Part	:Missile_fire2.part
	
		Magnet	T_Chest
		LookAt	T_Chest
	End

	
	Event
		EName 	Blast1
		Type	start
		At	WepR		
		Bhvr	:TurentBlastOrient1.bhvr
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	:MissleSmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
		LifeSpan	8

	End

	Event
		EName 	KickBackSmoke1
		Type	local
		At	WepR
		Bhvr	:TurentBlastOrientEndSmoke.bhvr
		Part1	:TurrentEndSmoke.part
		Part2	:MissleSmoke.part
		#Part4	WEAPONFX/Gunfire_MissleShell.Part
		LifeSpan	8
	End

	
End


#######################################################################################

Condition
	On 	Time
	Time 	70

	Event
		EName 	tracer1
		Type	Start
		At	WepL
		
		Geom	Fx_GunBurst
		Bhvr	:TurentBlastOrient.bhvr
		
		LifeSpan  2

	End


	Event
		EName 	Prime4
		Type	start
		At	WepL
		Bhvr	Behaviors/Missleprojectile.bhvr
		Geom	_5th_missileFrag
		Part1	:Missile_smoke.part
		Part2	:Missile_smoke2.part
		Part	:Missile_fire.part
		Part	:Missile_fire2.part
	
		Magnet	T_Chest
		LookAt	T_Chest
	End

	
	Event
		EName 	Blast1
		Type	start
		At	WepL		
		Bhvr	:TurentBlastOrient1.bhvr
		Part1	WEAPONFX/Gunfire_ShotgunMuzzflash.part
		Part2	:MissleSmoke.part
		Part3	WEAPONFX/Gunfire_ShotgunMuzzflashSmall.part
		Sound rocketlaunch 100.0 100.0 .9
		LifeSpan	8

	End

	Event
		EName 	KickBackSmoke1
		Type	local
		At	WepL
		Bhvr	:TurentBlastOrientEndSmoke.bhvr
		Part1	:TurrentEndSmoke.part
		Part2	:MissleSmoke.part
		#Part4	WEAPONFX/Gunfire_MissleShell.Part
		LifeSpan	8
	End

	
End

#######################################################################

Condition
	On 	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End

End

Condition
	On 	Prime1Hit

	Event
		Ename	Prime1
		Type	Destroy
	End

End

Condition
	On 	Prime2Hit

	Event
		Ename	Prime2
		Type	Destroy
	End

End

Condition
	On 	Prime3Hit

	Event
		Ename	Prime3
		Type	Destroy
	End

End

Condition
	On 	Prime4Hit

	Event
		Ename	Prime4
		Type	Destroy
	End

End

End				