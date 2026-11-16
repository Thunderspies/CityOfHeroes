#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	200

Condition
	On	Time
	Time	0

	Event
		EName	P
		Type	local
		At	T_wepR
		Bhvr	:SoulTrapObject.bhvr
		Sound soultraphit 80 80 .6
		Geom	FX_MetalBall
	End

	Event
		EName	Prime
		Type	start
		At	Chest
		Bhvr	behaviors/slowprjctLifeDrain.bhvr

		Part	:AngelFireSteam.part
		Part	:AngelFireLight.part
		Part	:AngelFireLight.part
		Part	:AngelFireLight.part
		Part	:AngelFireLight.part
		
		Magnet	P
		Lookat	P

	End

	Event
		Ename	rays
		Type	local
		At	Chest
		Part	:AngelFireLightRays.part
		Pmagnet	P
	End

	Event
		EName	glow
		Type	local
		At	P
		Part	:SoulTrapGlow.part

	End

	Event
		EName	pulse
		Type	SetLight
		At	T_WepR
		Bhvr	:SoulTrapPulse.bhvr
		Lifespan	120

	End

	Event
		EName	Pr
		Type	local
		At	P

		Part	:AngelFireDeviceEnergy.part
		Part	:AngelFireCoreEnergy.part

	End

End


Condition
	On	PrimeHit
	
	Event
		EName	Prime
		Type	Destroy
				
	End

	Event
		EName	P
		Type	Destroy
				
	End
	
	Event
		EName	pulse
		Type	Destroy
				
	End
	
	Event
		EName	rays
		Type	Destroy
				
	End
	
	Event
		EName	Pr
		Type	Destroy
				
	End

	Event
		Type	local
		At	T_WepR

		Bhvr	:SoulTrapObject.bhvr
		Part	:AngelFireLightRaysBright.part
		Part	:AngelFireDeviceEnergy.part
		Part	:AngelFireCoreEnergy.part
		Part	:AngelFireLightRays.part

		LifeSpan	15
	End

	Event
		EName	pulse2
		Type	SetLight
		At	T_WepR
		Bhvr	:SoulTrapPulse2.bhvr
		Lifespan	60

	End

End

End	


		