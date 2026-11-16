#########################################################
##	Fireweapon
########################################################
FxInfo

lifespan 100

######################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak1
		Type	local
		At	UlegR
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	LlegR
	End
	
	Event
		EName	Streak2
		Type	local
		At	UlegL
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	LlegL
	End

	Event
		EName	Streak3
		Type	local
		At	LlegR
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	FootR
	End
	
	Event
		EName	Streak4
		Type	local
		At	LlegL
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemSlashBodyGlows.part
		PMagnet	FootL
	End


	Event
		EName	Streak5
		Type	local
		At	FootR
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemMeleeStreak.part
		#Part2	:StrengthPulse.part
		#Part3	:StrengthGlow.part
		Sound footstomp2 100.0 100.0 .85
	End

	Event
		EName	Streak6
		Type	local
		At	FootL
		Bhvr	behaviors/genericparticlefade.bhvr
		Part1	:SlagGolemMeleeStreak.part
		#Part2	:StrengthPulse.part
		#Part3	:StrengthGlow.part
	End
End

Condition
	On 	Time
	Time 	26

	Event
		EName	flash
		Type	start
		At	mystic
		
		Part1	:SlagDust1.part
		Part2	:SlagDust2.part
		Part	:SlagDustRocks2.part
		Part3	:SlagRocks.part
		Part1	:AOEflash.part
		Part2	:AOEflash2.part
		Part3	:FootStompStreaks.part
		Part4	:SlagChunkHitFlash.part
		Part5	:LightBurstFlat.part

		Bhvr	Behaviors/CameraShake01.bhvr
	End
	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	mystic
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		40
			PhysForcePower		300
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End

Condition
	On 	Time
	Time 	28
	
	Repeat		1
	RepeatJitter	1
	
	Event
		EName	debris
		Type	StartPositOnly
		At	mystic
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	15
		LifespanJitter	5
				
	End
	
	Event
		HardwareOnly
		EName	debris
		Type	StartPositOnly
		At	mystic
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	15
		LifespanJitter	5
				
	End
	

End

Condition
	On 	Time
	Time 	29

	Event
		EName	flash2
		Type	start
		At	mystic
		
		Part	:SlagDustRocks2.part
		Part3	:SlagRocks.part
		Part2	:AOEflash3.part
	
	End

	Event
		EName	debris
		Type	StartPositOnly
		At	mystic
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:SlagDebrisExplode.bhvr
		Lifespan	15
		LifespanJitter	5
				
	End
	

End

Condition
	On 	Time
	Time 	30

	Event
		EName	Streak1
		Type	Destroy
		
	End
	
	Event
		EName	Streak2
		Type	Destroy
		
	End

	Event
		EName	Streak5
		Type	Destroy
		
	End

	Event
		EName	Streak6
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	42

	Event
		EName	Streak3
		Type	Destroy
		
	End

	Event
		EName	Streak4
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	100

	Event
		EName	all
		Type	Destroy
		
	End

End


End
#########################################################

