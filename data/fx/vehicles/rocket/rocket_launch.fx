#########################################################
##	Warburg Rocket - Launch
##

FxInfo
	
lifespan	435


Condition
	On Time
	Time  10

	Event
		Type	local
		At	origin
		Sound stone_loop 1200 1200 .9
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 300
	End
End


Condition
	On Time
	Time  10

	Event
		Type	local
		At	origin
		Sound smoke2_loop 1200 1200 .9
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 180
	End
End


Condition
	On Time
	Time  0

	Event
		Type	local
		At	origin
		Sound flamethrower2 1000 1000 .6
		#bhvr	behaviors/soundFade2.bhvr
		#Flags	PowerLoopingSound
		#Lifespan 60
	End
End


Condition
	On Time
	Time  120

	Event
		Type	local
		At	origin
		Sound Rocketlaunch5 1200.0 1200.0 .98
		#bhvr	behaviors/soundFade2.bhvr
		#Flags	PowerLoopingSound
		#Lifespan 60
	End
End



Condition
	On Time
	Time  0

	Event
		Type	local
		At	origin
		Sound rumble2_loop 1200 1200 .9
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 300
	End
End

Condition

	On Time 
	Time 0
	
	Event
		Ename	Start
		Type	Start #PositOnly
		At	Origin
		
	End

	Event
		EName 	Ignition
		Type	Local
		At	HandR
		ChildFX	:Rocket_ignition.fx
		Lifespan	115
	End
	
	Event
		EName   IgnitionGlow
		Type	Local
		At	Start
		Splat	ember.tga
		Bhvr	:Rocket_GlowSplat.bhvr
		Lifespan	120
	End

	Event
		ENAME	Glow
		Type	SetLight	
		Bhvr	:Rocket_Glow.bhvr
		
		Lifespan 180
	End

End
	
Condition

	On Time 
	Time 10

	Event
		Ename	ExplosiveForce
		Type	Start
		At	HandR
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		160
			PhysForcePower		50
			PhysForcePowerJitter	25
		End

		Lifespan	180

	End

	Event
		EName 	Thruster
		Type	Local
		At	F1_L
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		
		Lifespan	180
	End

	Event
		EName 	Thruster
		Type	Local
		At	F1_R
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	F2_L
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	F2_R
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	F2_L
		BhvrOverride
			PositionOffset	5.4 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	F2_R
		BhvrOverride
			PositionOffset	-5.4 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	FootL
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	FootR
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	LlegL
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	LlegR
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	UlegL
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	UlegR
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	RingL
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	RingR
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	ToeL
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End
	Event
		EName 	Thruster
		Type	Local
		At	ToeR
		BhvrOverride
			PositionOffset	0 0 0
		End
#		Part1	:Rocket_thruster_smoke.part
		Part2	:Rocket_thruster_fire.part
		Part2	:Rocket_thruster_sparks.part
		Lifespan	180
	End

End
Condition

	On Time 
	Time 100
	
	Event
		EName 	Cloud
		Type	Local
		At	HandR
		ChildFX	:Rocket_Clouds.fx
		Part2	:Rocket_booster_fire.part
		Part1	:Rocket_booster_smoke.part
		Lifespan	310
	End
	
	Event
		EName 	Cloud
		Type	Local
		At	UarmL
		ChildFX	:Rocket_Clouds.fx
		Part2	:Rocket_booster_fire.part
		Part1	:Rocket_booster_smoke.part
		Lifespan	300
	End
	
	Event
		EName 	Cloud
		Type	Local
		At	UarmR
		ChildFX	:Rocket_Clouds.fx
		Part2	:Rocket_booster_fire.part
		Part1	:Rocket_booster_smoke.part
		Lifespan	280
	End
	
	Event
		EName 	Cloud
		Type	Local
		At	LarmL
		ChildFX	:Rocket_Clouds.fx
		Part2	:Rocket_booster_fire.part
		Part1	:Rocket_booster_smoke.part
		Lifespan	300
	End
	
	Event
		EName 	Cloud
		Type	Local
		At	LarmR
		ChildFX	:Rocket_Clouds.fx
		Part2	:Rocket_booster_fire.part
		Part1	:Rocket_booster_smoke.part
		Lifespan	280
	End
End

Condition

	On Time 
	Time 60
		
	Event
		EName 	SteamHookup
		Type	Start
		At	Face
	End
	
	Event
		EName 	Steam
		Type	Local
		At	SteamHookup
		BhvrOverride
			PositionOffset	0 -5 0
		End
		Part1	:Gantry_Steam.part
		Lifespan	325
	End
	
End

End