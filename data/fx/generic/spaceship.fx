#########################################################
##	template

FxInfo

Input  
	Inpname	LarmL
End

Input  
	Inpname	Waist
End

Input  
	Inpname	Chest
End

Input  
	Inpname	Neck
End

Input  
	Inpname	Head
End

Input  
	Inpname	Col_R
End

Input  
	Inpname	Col_L
End

Input  
	Inpname	UarmL
End

Input  
	Inpname	UarmR
End

Input  
	Inpname	ULegL
End

Input  
	Inpname	ULegR
End

Input  
	Inpname	LlegL
End

Input  
	Inpname	LlegR
End

Input  
	Inpname	FootR
End

Input  
	Inpname	FootL
End


Input  
	Inpname	ToeL
End

Input  
	Inpname	ToeR
End

Input  
	Inpname	Face
End

Input  
	Inpname	Breast
End

Input  
	Inpname	Belt
End

Input  
	Inpname	GloveL
End

Input  
	Inpname	Hips
End

############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	topGlobe
		Type	local
		At	hips
		Geom	TopGlobe
		sound	spaceship6_loop 4800.0 800.0 .85
	End

	Event
		EName	bottemGlobe
		Type	local
		At	hips
		Geom	BottemGlobe
		
	End

	

			
####################################################################
##InsideRing
###################################################################
	
	Event
		EName	lightsRotating
		Type	local
		At	hips
		Bhvr	behaviors/spin5.bhvr
		Geom	BoxRotate
	End

	Event
		EName	glow11
		Type	local
		At	lightsRotating
		altpiv	7
		Part4	EnemyFX/Rikti/RiktiSmallBeamGlowFlat.part
		Part5	EnemyFX/Rikti/RiktiBeamGlow.part

	End

	Event
		
		Type	local
		At	lightsRotating
		altpiv	1
		Part1	EnemyFX/Rikti/GreenLightPulsing2.part
		Part2	EnemyFX/Rikti/WhiteLightPulsing2.part

	End

	Event
		Type	local
		At	lightsRotating
		altpiv	2
		Part1	EnemyFX/Rikti/GreenLightPulsing2.part
		Part2	EnemyFX/Rikti/WhiteLightPulsing2.part
	End

	Event
		
		Type	local
		At	lightsRotating
		altpiv	3
		Part1	EnemyFX/Rikti/GreenLightPulsing2.part
		Part2	EnemyFX/Rikti/WhiteLightPulsing2.part
	End

	Event
		Type	local
		At	lightsRotating
		altpiv	4
		Part1	EnemyFX/Rikti/GreenLightPulsing2.part
		Part2	EnemyFX/Rikti/WhiteLightPulsing2.part
	End

	Event
		
		Type	local
		At	lightsRotating
		altpiv	5
		Part1	EnemyFX/Rikti/GreenLightPulsing2.part
		Part2	EnemyFX/Rikti/WhiteLightPulsing2.part
	End

	Event
		Type	local
		At	lightsRotating
		altpiv	6
		Part1	EnemyFX/Rikti/GreenLightPulsing2.part
		Part2	EnemyFX/Rikti/WhiteLightPulsing2.part
	End

End


##################################################################
##Antenna Lights
#####################################################
	
	

Condition
	On 	Time
	Time 	35

	Event
		EName	RunningLights09
		Type	local
		At	LarmL
		Part1	EnemyFX/Rikti/FastGreenLightPulsing.part
		Part2	EnemyFX/Rikti/FastRiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/RiktiRunningLightRings.part
	End

End

Condition
	On 	Time
	Time 	38

	Event
		EName	RunningLights08
		Type	local
		At	UarmR
		Part1	EnemyFX/Rikti/FastGreenLightPulsing.part
		Part2	EnemyFX/Rikti/FastRiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/RiktiRunningLightRings.part
	End

End

Condition
	On 	Time
	Time 	39

	Event
		EName	RunningLights06
		Type	local
		At	Col_L
		Part1	EnemyFX/Rikti/FastGreenLightPulsing.part
		Part2	EnemyFX/Rikti/FastRiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/RiktiRunningLightRings.part
	End

End

Condition
	On 	Time
	Time 	42

	Event
		EName	RunningLights04
		Type	local
		At	Head
		Part1	EnemyFX/Rikti/FastGreenLightPulsing.part
		Part2	EnemyFX/Rikti/FastRiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/RiktiRunningLightRings.part
	End

End

Condition
	On 	Time
	Time 	43

	Event
		EName	RunningLights03
		Type	local
		At	Neck
		Part1	EnemyFX/Rikti/FastGreenLightPulsing.part
		Part2	EnemyFX/Rikti/FastRiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/RiktiRunningLightRings.part
	End

End

Condition
	On 	Time
	Time 	46

	Event
		EName	RunningLights01
		Type	local
		At	Waist
		Part1	EnemyFX/Rikti/FastGreenLightPulsing.part
		Part2	EnemyFX/Rikti/FastRiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/RiktiRunningLightRings.part
	End

				
End

Condition
	On 	Time
	Time 	0

	Event
		EName	RunningLights02
		Type	local
		At	Chest
		Part1	EnemyFX/Rikti/GreenLightPulsingBottem.part
		Part2	EnemyFX/Rikti/RiktiShipWhiteLightPulsingBottem.part
		Part3	EnemyFX/Rikti/FastRiktiRunningLightRings.part
	End

	Event
		EName	RunningLights05
		Type	local
		At	Col_R
		Part1	EnemyFX/Rikti/GreenLightPulsingBottem.part
		Part2	EnemyFX/Rikti/RiktiShipWhiteLightPulsingBottem.part
		Part3	EnemyFX/Rikti/FastRiktiRunningLightRings.part
	End

	Event
		EName	RunningLights07
		Type	local
		At	UarmL
		Part1	EnemyFX/Rikti/GreenLightPulsingBottem.part
		Part2	EnemyFX/Rikti/RiktiShipWhiteLightPulsingBottem.part
		Part3	EnemyFX/Rikti/FastRiktiRunningLightRings.part
	End

#############################################################
##ShipLights
##############################################################
	
	Event
	
		EName	ShipLights01
		Type	local
		At	ToeR
		Part1	EnemyFX/Rikti/GreenLightPulsing.part
		Part2	EnemyFX/Rikti/RiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/GreenLightPulsing.part
		Part4	EnemyFX/Rikti/RiktiShipGlowStreaks.part
	End

	Event
	
		EName	ShipLights02
		Type	local
		At	ToeL
		Part1	EnemyFX/Rikti/GreenLightPulsing.part
		Part2	EnemyFX/Rikti/RiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/GreenLightPulsing.part
		Part4	EnemyFX/Rikti/RiktiShipGlowStreaks.part
	End

	Event
	
		EName	ShipLights03
		Type	local
		At	Face
		Part1	EnemyFX/Rikti/GreenLightPulsing.part
		Part2	EnemyFX/Rikti/RiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/GreenLightPulsing.part
		Part4	EnemyFX/Rikti/RiktiShipGlowStreaks.part

	End

	Event
	
		EName	ShipLights04
		Type	local
		At	Breast
		Part1	EnemyFX/Rikti/GreenLightPulsing.part
		Part2	EnemyFX/Rikti/RiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/GreenLightPulsing.part
		Part4	EnemyFX/Rikti/RiktiShipGlowStreaks.part

	End

	Event
	
		EName	ShipLights05
		Type	local
		At	Belt
		Part1	EnemyFX/Rikti/GreenLightPulsing.part
		Part2	EnemyFX/Rikti/RiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/GreenLightPulsing.part
		Part4	EnemyFX/Rikti/RiktiShipGlowStreaks.part

	End

	Event
	
		EName	ShipLights06
		Type	local
		At	GloveL
		Part1	EnemyFX/Rikti/GreenLightPulsing.part
		Part2	EnemyFX/Rikti/RiktiShipWhiteLightPulsing.part
		Part3	EnemyFX/Rikti/GreenLightPulsing.part
		Part4	EnemyFX/Rikti/RiktiShipGlowStreaks.part


	End


End

##################################################################
##Gasses
##########################################################
	
Condition
	On 	cycle
	Time 	30
	Chance	.20
	
	Event
		EName	Vent01
		Type	posit
		At	UlegR
		Part1	EnemyFX/Rikti/ShipVentGasMist.part
		Part2	EnemyFX/Rikti/ShipVentGas.part
		Lifespan 80
	End

End

Condition
	On 	cycle
	Time 	30
	Chance	.20

	Event
		EName	Vent02
		Type	posit
		At	UlegL
		Part1	EnemyFX/Rikti/ShipVentGasMist.part
		Part2	EnemyFX/Rikti/ShipVentGas.part
		Lifespan 80
	End

End

Condition
	On 	cycle
	Time 	30
	Chance	.20

	Event
		EName	Vent03
		Type	posit
		At	LlegR
		Part1	EnemyFX/Rikti/ShipVentGasMist.part
		Part2	EnemyFX/Rikti/ShipVentGas.part
		Lifespan 80
	End

End

Condition
	On 	cycle
	Time 	30
	Chance	.20

	Event
		EName	Vent04
		Type	posit
		At	LlegL
		Part1	EnemyFX/Rikti/ShipVentGasMist.part
		Part2	EnemyFX/Rikti/ShipVentGas.part
		Lifespan 80
	End

End

Condition
	On 	cycle
	Time 	30
	Chance	.20

	Event
		EName	Vent05
		Type	posit
		At	FootR
		Part1	EnemyFX/Rikti/ShipVentGasMist.part
		Part2	EnemyFX/Rikti/ShipVentGas.part
		Lifespan 80
	End

End

Condition
	On 	cycle
	Time 	30
	Chance	.20
	Event
		EName	Vent06
		Type	posit
		At	FootL
		Part1	EnemyFX/Rikti/ShipVentGasMist.part
		Part2	EnemyFX/Rikti/ShipVentGas.part
		Lifespan 80
	End

End




End		