################x#########################################
##	chill touch hit
FxInfo

LifeSpan	105	#75

############################################################

Condition
	On 	Time
	Time 	60	#30

	
	Event
		EName	PrimeBlast
		Type	start
		At	WepL
		Bhvr	:SkullBlastOffset.bhvr
		Part	:SkullRingsBlackBlast.part
		Part	:SkullRingsBlast.part
		Part	:SkullRings2Blast.part
		
		LifeSpan	3
	
	End


	Event	

		ENAME	skull
		Type	start
		At	head
		Anim	Fx_Skull
		bhvr	:SkullAlpha.bhvr
		Sound aura4 100 100 1.0
		
		LifeSpan	15

	End

	Event	

		ENAME	LeftEye
		Type	local
		At	Skull
		bhvr	:SkullLeftEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End

	Event	

		ENAME	RightEye
		Type	local
		At	Skull

		bhvr	:SkullRightEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End

	Event
		EName 	Orb
		Type	Local 
		At	Skull
		
		bhvr	:SkullRightEyeOffset.bhvr
		part1	:FearMist.part
		part2	:FearMist2.part
		Part3	:FearTenticles.part
		Part4	:FearTenticles2.part

		Part	:SkullRingsBlack.part
		Part	:SkullRings.part
		Part	:SkullRings2.part


		LifeSpan	33
		
	End






	Event	

		ENAME	skull1
		Type	start
		At	head
		Anim	Fx_Skull
		bhvr	:SkullAlpha3.bhvr
		Sound aura4 80 80 .8
		
		LifeSpan	15

	End

	Event	

		ENAME	LeftEye
		Type	local
		At	Skull1
		bhvr	:SkullLeftEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End

	Event	

		ENAME	RightEye
		Type	local
		At	Skull1

		bhvr	:SkullRightEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End


	Event
		EName 	Orb
		Type	Local 
		At	Skull1
		
		bhvr	:SkullRightEyeOffset.bhvr
		part1	:FearMist.part
		part2	:FearMist2.part
		Part3	:FearTenticles.part
		Part4	:FearTenticles2.part
		Part	:SkullRingsBlack.part
		Part	:SkullRings.part
		Part	:SkullRings2.part


		LifeSpan	35
		
	End






	Event	

		ENAME	skull2
		Type	start
		At	head
		Anim	Fx_Skull
		bhvr	:SkullAlpha4.bhvr
		#Sound	aura4 80 30 .8
		
		LifeSpan	15

	End

	Event	

		ENAME	LeftEye
		Type	local
		At	Skull2
		bhvr	:SkullLeftEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End

	Event	

		ENAME	RightEye
		Type	local
		At	Skull2

		bhvr	:SkullRightEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End


	Event
		EName 	Orb
		Type	Local 
		At	Skull2
		
		bhvr	:SkullRightEyeOffset.bhvr
		part1	:FearMist.part
		part2	:FearMist2.part
		Part3	:FearTenticles.part
		Part4	:FearTenticles2.part
		Part	:SkullRingsBlack.part
		Part	:SkullRings.part
		Part	:SkullRings2.part


		LifeSpan	35
		
	End






	Event	

		ENAME	skull3
		Type	start
		At	head
		Anim	Fx_Skull
		bhvr	:SkullAlpha5.bhvr
		#Sound	aura4 80 30 .8
		
		LifeSpan	15

	End

	Event	

		ENAME	LeftEye
		Type	local
		At	Skull3
		bhvr	:SkullLeftEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End

	Event	

		ENAME	RightEye
		Type	local
		At	Skull3

		bhvr	:SkullRightEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End


	Event
		EName 	Orb
		Type	Local 
		At	Skull3
		
		bhvr	:SkullRightEyeOffset.bhvr
		part1	:FearMist.part
		part2	:FearMist2.part
		Part3	:FearTenticles.part
		Part4	:FearTenticles2.part
		Part	:SkullRingsBlack.part
		Part	:SkullRings.part
		Part	:SkullRings2.part


		LifeSpan	35
		
	End






	Event	

		ENAME	skull7
		Type	start
		At	head
		Anim	Fx_Skull
		bhvr	:SkullAlpha7.bhvr
		#Sound	aura4 80 30 .8
		
		LifeSpan	15

	End

	Event	

		ENAME	LeftEye
		Type	local
		At	Skull7
		bhvr	:SkullLeftEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End

	Event	

		ENAME	RightEye
		Type	local
		At	Skull7

		bhvr	:SkullRightEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End

	Event
		EName 	Orb
		Type	Local 
		At	Skull7
		
		bhvr	:SkullRightEyeOffset.bhvr
		part1	:FearMist.part
		part2	:FearMist2.part
		Part3	:FearTenticles.part
		Part4	:FearTenticles2.part
		Part	:SkullRingsBlack.part
		Part	:SkullRings.part
		Part	:SkullRings2.part

		LifeSpan	35
		
	End








	Event	

		ENAME	skull6
		Type	start
		At	head
		Anim	Fx_Skull
		bhvr	:SkullAlpha6.bhvr
		#Sound	aura4 80 30 .8
		
		LifeSpan	15

	End

	Event	

		ENAME	LeftEye
		Type	local
		At	Skull6
		bhvr	:SkullLeftEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End

	Event	

		ENAME	RightEye
		Type	local
		At	Skull6

		bhvr	:SkullRightEyeOffset.bhvr
		Part	POWERS\DarknessControl\DaemonEyes.part
		Part	POWERS\DarknessControl\DaemonPupil.part
		
		LifeSpan	31
	End

	Event
		EName 	Orb
		Type	Local 
		At	Skull6
		
		bhvr	:SkullRightEyeOffset.bhvr
		part1	:FearMist.part
		part2	:FearMist2.part
		Part3	:FearTenticles.part
		Part4	:FearTenticles2.part
		Part	:SkullRingsBlack.part
		Part	:SkullRings.part
		Part	:SkullRings2.part

		LifeSpan	35
		
	End

End


Condition
	On 	Time
	Time 	77	#47
	
	
	Event
		EName	PrimeBlast
		Type	start
		At	WepL
		Bhvr	:SkullBlastOffset.bhvr
		Part	:SkullRingsBlackBlast.part
		Part	:SkullRingsBlast.part
		Part	:SkullRings2Blast.part
		
		LifeSpan	3
	
	End

End

Condition
	On 	Time
	Time 	63	#33
	
		Event
		EName	PrimeBlast
		Type	start
		At	WepL
		Bhvr	:SkullBlastOffset.bhvr
		Part	:SkullRingsBlackBlast.part
		Part	:SkullRingsBlast.part
		Part	:SkullRings2Blast.part
		
		LifeSpan	3
	
	End

End

Condition
	On 	Time
	Time 	70	#40

	Event
	
		EName	Prime
		Type	start
		At	WepL
		Bhvr	:Skullprojectiles.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
		
	
	End

	Event
		EName	SkullRotate
		Type	local
		At	Prime
		Bhvr	behaviors/SkullprojectilesRotate.bhvr
			
	
	End

	Event
		EName	SkullOffset
		Type	local
		At	SkullRotate
		Bhvr	behaviors/SkullprojectilesRotate.bhvr
		Geom	Skull
		
		
	
	End
	
	Event
		EName	eye1
		Type	local
		At	SkullOffset
		 altpiv	2
		Part1	POWERS\DarknessControl\DaemonEyes2.part
		Part2	POWERS\DarknessControl\DaemonPupil2.part
	End

	Event
		EName	eye2
		Type	local
		At	SkullOffset
		 altpiv	3
		Part1	POWERS\DarknessControl\DaemonEyes2.part
		Part2	POWERS\DarknessControl\DaemonPupil2.part
	End


	Event
		Type	local
		At	SkullOffset
		altpiv	1

		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:Skullsmoke.part
		Part	:Skullsmoke.part
			
	End

##########################################################

	Event
		EName	Prime1
		Type	start
		At	WepL
		Bhvr	:Skullprojectiles1.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
		
	
	End

	Event
		EName	SkullRotate1
		Type	local
		At	Prime1
		Bhvr	behaviors/SkullprojectilesRotate1.bhvr
			
	
	End

	Event
		EName	SkullOffset1
		Type	local
		At	SkullRotate1
		Bhvr	behaviors/SkullprojectilesRotate1.bhvr
		Geom	Skull
		
		
	
	End
	
	Event
		EName	1
		Type	local
		At	SkullOffset1
		 altpiv	2
		Part1	POWERS\DarknessControl\DaemonEyes2.part
		Part2	POWERS\DarknessControl\DaemonPupil2.part
	End

	Event
		EName	2
		Type	local
		At	SkullOffset1
		 altpiv	3
		Part1	POWERS\DarknessControl\DaemonEyes2.part
		Part2	POWERS\DarknessControl\DaemonPupil2.part
	End


	Event
		Type	local
		At	SkullOffset1
		altpiv	1

		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:Skullsmoke.part
		Part	:Skullsmoke.part
			
	End






	Event
		EName	Prime2
		Type	start
		At	WepL
		Bhvr	:Skullprojectiles2.bhvr
		Magnet	T_Chest
		LookAt  T_Chest
		
	
	End

	Event
		EName	SkullRotate2
		Type	local
		At	Prime1
		Bhvr	behaviors/SkullprojectilesRotate2.bhvr
			
	
	End

	Event
		EName	SkullOffset2
		Type	local
		At	SkullRotate2
		Bhvr	behaviors/SkullprojectilesRotate2.bhvr
		Geom	Skull
		
		
	
	End
	
	Event
		EName	e1
		Type	local
		At	SkullOffset2
		 altpiv	2
		Part1	POWERS\DarknessControl\DaemonEyes2.part
		Part2	POWERS\DarknessControl\DaemonPupil2.part
	End

	Event
		EName	e2
		Type	local
		At	SkullOffset2
		 altpiv	3
		Part1	POWERS\DarknessControl\DaemonEyes2.part
		Part2	POWERS\DarknessControl\DaemonPupil2.part
	End


	Event
		Type	local
		At	SkullOffset2
		altpiv	1

		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:SkullTenticles2.part
		Part	:Skullsmoke.part
		Part	:Skullsmoke.part
					
	End

End

#################################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

	Event
		EName 	Prime1
		Type	Destroy

	End

	Event
		EName 	Prime2
		Type	Destroy

	End
		
	Event
		EName 	Eye1
		Type	Destroy

	End

	Event
		EName 	Eye2
		Type	Destroy

	End
		
	Event
		EName 	1
		Type	Destroy

	End

	Event
		EName 	2
		Type	Destroy

	End
		
	Event
		EName 	E1
		Type	Destroy

	End

	Event
		EName 	E2
		Type	Destroy

	End
	


End

End




















		