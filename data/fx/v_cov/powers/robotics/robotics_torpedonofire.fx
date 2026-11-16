#########################################################
##	template

FxInfo

LifeSpan		300

#########################################################
####################### AUDIO ###########################
#########################################################




Condition
	On	Time
	Time	20


	Event
		Type	Local
		At 	Origin
		Sound Rocketlaunch6 110 110 .89
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	25

	Event
		ENAME	TorpedoHookup
		Type	local
		At	back
		Geom	missleHookup

	End

	Event
		Type	local
		At	TorpedoHookup
		Altpiv	1

		Part	:Torpedo_SmokeUp.part
		Part	:Torpedo_fireBurst.part

		LifeSpan	25

	End

	Event
		Type	local
		At	TorpedoHookup
		Altpiv	2

		Part	:Torpedo_SmokeUp.part
		Part	:Torpedo_fireBurst.part

		LifeSpan	25

	End

End

Condition
	On	Time
	Time	25


	Event
		ENAME	Prime
		Type	start
		At	TorpedoHookup
		Altpiv	1
		BHVR	:SpinProjectile.Bhvr
		Geom	Robotics_Torpedo
		Part	:Torpedo_smoke.part
		Part	:Torpedo_smoke2.part
		Part	:Torpedo_fire_Small.part
		Magnet	Target
		Lookat	Target
	End

	Event
		ENAME	Prime2
		Type	start
		At	TorpedoHookup
		Altpiv	1
		BHVR	:SpinProjectileSlow.Bhvr
		Magnet	Target
		Lookat	Target
	End

	Event
		Type	local
		At	Prime2

		BHVR	:AirOffset.Bhvr
		Part	:Torpedo_AirTrail2.part

	End

	Event
		Type	local
		At	Prime2

		BHVR	:AirOffset2.Bhvr
		Part	:Torpedo_AirTrail2.part

	End

End

Condition
	On	Time
	Time	29

	Event
		Type	start
		At	chest
		BHVR	:TorpedoFade.bhvr
		ChildFx	:Robotics_TorpedoChildNoFire.fx
		Lifespan	300
	End
End

Condition
	On	Time
	Time	35

	Event
		Type	start
		At	chest
		BHVR	:TorpedoFade.bhvr
		ChildFx	:Robotics_TorpedoChildNoFire.fx
		Lifespan	300
	End
End

Condition
	On	Time
	Time	38

	Event
		Type	start
		At	chest
		BHVR	:TorpedoFade.bhvr
		ChildFx	:Robotics_TorpedoChildNoFire.fx
		Lifespan	300
	End
End

Condition
	On	Time
	Time	50

	Event
		Type	start
		At	chest
		BHVR	:TorpedoFade.bhvr
		ChildFx	:Robotics_TorpedoChildNoFire.fx
		Lifespan	300
	End
End

Condition
	On	Time
	Time	43

	Event
		Type	start
		At	chest
		#BHVR	:TorpedoFade.bhvr
		ChildFx	:Robotics_TorpedoChildNoFire.fx
		Lifespan	300
	End
End

Condition
	On	Time
	Time	38

	Event
		Type	start
		At	chest
		BHVR	:TorpedoFade.bhvr
		ChildFx	:Robotics_TorpedoChildNoFire.fx
		Lifespan	300
	End
End

Condition
	On	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End
End


Condition
	On	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy

	End


End

End