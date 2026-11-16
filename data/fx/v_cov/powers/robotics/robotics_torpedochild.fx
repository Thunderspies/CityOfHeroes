#########################################################
##	template

FxInfo

LifeSpan	300
#########################################################
####################### AUDIO ###########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		#Sound	Glow3_loop 80 30 .26
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0
	
	Event	
		ENAME	TorpedoHookup
		Type	local
		At	back
		Geom	missleHookup

	End


	Event	
		ENAME	Prime3
		Type	start
		At	TorpedoHookup
		Altpiv	1
		BHVR	:SpinProjectile.Bhvr
		Geom	Robotics_Torpedo

		Part	:Torpedo_smoke.part
		Part	:Torpedo_smoke2.part
		Part	:Torpedo_fire.part
		Part	:Torpedo_fireBig.part
		Part	:Torpedo_fireCore.part
		Magnet	Target
		Lookat	Target
	End

	Event	
		ENAME	Prime4
		Type	start
		At	TorpedoHookup
		Altpiv	1
		BHVR	:SpinProjectileSlow.Bhvr
		Magnet	Target
		Lookat	Target
	End

	Event	
		Type	local
		At	Prime4
		
		BHVR	:AirOffset.Bhvr
		Part	:Torpedo_AirTrail.part

	End

	Event	
		Type	local
		At	Prime4
		
		BHVR	:AirOffset2.Bhvr
		Part	:Torpedo_AirTrail.part

	End

###########################################################################################

	Event	
		ENAME	Prime
		Type	start
		At	TorpedoHookup
		Altpiv	2
		BHVR	:SpinProjectile.Bhvr
		Geom	Robotics_Torpedo

		Part	:Torpedo_smoke.part
		Part	:Torpedo_smoke2.part
		Part	:Torpedo_fire.part
		Part	:Torpedo_fireBig.part
		Part	:Torpedo_fireCore.part
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
		Part	:Torpedo_AirTrail.part

	End

	Event	
		Type	local
		At	Prime2
		
		BHVR	:AirOffset2.Bhvr
		Part	:Torpedo_AirTrail.part

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

Condition
	On	Prime3Hit
	
	
	Event
		EName 	Prime3
		Type	Destroy

	End
End


Condition
	On	Prime4Hit
	
	Event
		EName 	Prime4
		Type	Destroy

	End


End

End		