#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	180


Input  
	InpName	Origin
End


Input  
	InpName	Target
End

Input  
	Inpname	UarmL
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	LarmR
End

Input  
	Inpname	UarmR
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Hips
End

###########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Local
		At 	origin
		Sound Widowspincast3 80 80 .86
	End
End

Condition
	On	Time
	Time	0
	
	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
		
		Sound nightFall2 100.0 100.0 .85
		
		PMagnet	LarmR
		LifeSpan	50
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
				
		PMagnet	LarmL
		LifeSpan	50
	End


End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
		
		PMagnet	WepR
		LifeSpan	50
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	POWERS/DarknessControl/DarkBlastTenticle.part
		Part3	POWERS/DarknessControl/DarkHandsEmberb.part
		
		PMagnet	WepL
		LifeSpan	50
	End

End

################  Start   #####################

Condition
	On	Time
	Time	10
	Event
		EName 	binder7 
		Type	local
		At	UarmR
		Bhvr	:soulBinderArm2.bhvr
		Lifespan	20
	End

	Event
		EName 	Offset7 
		Type	local
		At	binder7
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset7
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset7
		  AltPiv	4
		Part	:soulFire0.part
	End

End


################   2   ##########################

Condition
	On	Time
	Time	10

	Event
		EName 	binder8 
		Type	local
		At	UarmL
		Bhvr	:soulBinderArm1.bhvr
		Lifespan	20
	End

	Event
		EName 	Offset8 
		Type	local
		At	binder8
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset8
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset8
		  AltPiv	4
		Part	:soulFire0.part
	End

End

################   3   ##########################

Condition
	On	Time
	Time	18
	Event
		EName 	binder9 
		Type	local
		At	larmR
		Bhvr	:soulBinderArm2.bhvr

		Lifespan	20
	End

	Event
		EName 	Offset9 
		Type	local
		At	binder9
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset9
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset9
		  AltPiv	4
		Part	:soulFire0.part
	End

End


################   4   ##########################

Condition
	On	Time
	Time	18

	Event
		EName 	binder10 
		Type	local
		At	larmL
		Bhvr	:soulBinderArm1.bhvr
		
		Lifespan	20
	End

	Event
		EName 	Offset10 
		Type	local
		At	binder10
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset10
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset10
		  AltPiv	4
		Part	:soulFire0.part
	End

End

##################  5   ##########################

Condition
	On	Time
	Time	25
	Event
		EName 	binder1 
		Type	local
		At	handR
		Bhvr	:soulBinderArm2.bhvr
		BhvrOverride
			InitialVelocity 0 0 0
			InitialVelocityJitter	0 0 0
			SpinJitter	0.05 0.015 0.015
		End
		
		Lifespan	25

	End

	Event
		EName 	Offset1 
		Type	local
		At	binder1
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset1
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset9
		  AltPiv	4
		Part	:soulFire0.part
	End

End

##################  6   ##########################

Condition
	On	Time
	Time	25
	Event
		EName 	binder2 
		Type	local
		At	handL
		Bhvr	:soulBinderArm1.bhvr
		BhvrOverride
			InitialVelocity 0 0 0
			InitialVelocityJitter	0 0 0
			SpinJitter	0.05 0.015 0.015
		End
		
		Lifespan	25

	End

	Event
		EName 	Offset2 
		Type	local
		At	binder2
		Bhvr	:binderOffset.bhvr
		Part	:soulGlow0.part
		Part	:soulFire0.part
		Part	:soulSmoke0.part
		Geom	stormFXsoul
	End

	Event
		EName 	Arm1 
		Type	local
		At	Offset2
		  AltPiv	2
		Part	:soulFire0.part
	End

	Event
		EName 	Arm2 
		Type	local
		At	Offset2
		  AltPiv	4
		Part	:soulFire0.part
	End

End


############   Hands   ###########


Condition
	On	Time
	Time	25
	
	Event
		EName 	HandR 
		Type	local
		At	WepR
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke0.part

		Lifespan	23
	End


	Event
		EName 	HandL 
		Type	local
		At	WepL
		Part	:soulGlow1.part
		Part	:soulFire2.part
		Part	:soulSmoke0.part

		Lifespan	23
	End

End


##########  Projectiles  ################

Condition
	On	Time
	Time	30


	Event
		EName	Prime
		Type	start
		At	WepL
		#Part1	POWERS/DarknessControl/DarknessRapidTenticles.part
		Part4	POWERS/DarknessControl/DarknessRapidTenticlesCore.part
		Part2	POWERS/DarknessControl/DarknessRapidCloud.part
		#Part3	POWERS/DarknessControl/DarknessRapidCloud2.part
		#Part5	POWERS/DarknessControl/SmallTenticlesHead.part
		Part	:soulSmoke1.part

		Part	:soulGlow0.part

		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End


End


Condition
	On	Time
	Time	37


	Event
		EName	Prime1
		Type	start
		At	WepR

		Part1	POWERS/DarknessControl/DarknessRapidTenticles.part
		Part4	POWERS/DarknessControl/DarknessRapidTenticlesCore.part
		Part2	POWERS/DarknessControl/DarknessRapidCloud.part
		Part3	POWERS/DarknessControl/DarknessRapidCloud2.part
		#Part5	POWERS/DarknessControl/SmallTenticlesHead.part
		Part	:soulSmoke1.part

		Part	:soulGlow0.part

		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

End

Condition
	On	Time
	Time	57


	Event
		EName	Prime2
		Type	start
		At	WepL
		
		Part1	POWERS/DarknessControl/DarknessRapidTenticles.part
		Part4	POWERS/DarknessControl/DarknessRapidTenticlesCore.part
		Part2	POWERS/DarknessControl/DarknessRapidCloud.part
		Part3	POWERS/DarknessControl/DarknessRapidCloud2.part
		#Part5	POWERS/DarknessControl/SmallTenticlesHead.part
		#Part	:soulSmoke1.part

		Part	:soulGlow0.part

		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End


End

Condition
	On	Time
	Time	49

	Event
		EName	Prime3
		Type	start
		At	WepR

		Part1	POWERS/DarknessControl/DarknessRapidTenticles.part
		Part4	POWERS/DarknessControl/DarknessRapidTenticlesCore.part
		Part2	POWERS/DarknessControl/DarknessRapidCloud.part
		Part3	POWERS/DarknessControl/DarknessRapidCloud2.part
		#Part5	POWERS/DarknessControl/SmallTenticlesHead.part
		#Part	:soulSmoke1.part

		Part	:soulGlow0.part

		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	
End

Condition
	On	Time
	Time	44

	Event
		EName	Prime4
		Type	start
		At	WepL

		Part1	POWERS/DarknessControl/DarknessRapidTenticles.part
		Part4	POWERS/DarknessControl/DarknessRapidTenticlesCore.part
		Part2	POWERS/DarknessControl/DarknessRapidCloud.part
		Part3	POWERS/DarknessControl/DarknessRapidCloud2.part
		#Part5	POWERS/DarknessControl/SmallTenticlesHead.part
		#Part	:soulSmoke1.part

		Part	:soulGlow0.part

		bhvr	behaviors/projectile1.bhvr
		Magnet	Target
		LookAt  Target
	
	End

	
End

Condition
	On 	Time
	Time	40

	Event
		EName 	HandGlowRight1
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft1
		Type	Destroy
		

	End

End

Condition
	On 	Time
	Time	60

	Event
		EName 	HandGlowRight2
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft2
		Type	Destroy
		

	End

	Event
		EName 	HandGlowRight22
		Type	Destroy
		

	End

	Event
		EName 	HandGlowLeft22
		Type	Destroy
		

	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy
		

	End

End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
		

	End

End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
		

	End


End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy
		

	End

End


End			