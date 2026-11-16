#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	200

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	head
End

Input  
	InpName	T_Chest
End

#Input  
#	Inpname	WepR
#End
#
#Input  
#	Inpname	WepL
#End

###########################################################

Condition

	On Time 
	Time 0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01	
		bhvr	behaviors/PsionicsScale3.bhvr
		
	End

	Event

		
		Type	Local
		At	core
		altPiv	1
		Bhvr	behaviors/XRayBeam.bhvr
		Part1	:XRayEyeGlowRed.Part
		Part2	:XRayEyeRingsRed.Part
		
	End

	Event
		
		Type	Local
		At	core
		altPiv	3
		Bhvr	behaviors/XRayBeam.bhvr
		Part1	:XRayEyeGlowRed.Part
		Part2	:XRayEyeRingsRed.Part
		Sound radiation3_loop 80 80 .4
	End

End



Condition
	On	Time
	Time	6


	Event
		Type	Local
		At	Core
		Sound xray5 80 80 .9
	End
End

Condition
	On	Time
	Time	30

	Event
		EName 	Prime
		Type	Start 
		At	Mystic
		Bhvr	behaviors/Fastprojectile4.bhvr

		Magnet	T_Chest
		LookAt	T_Chest
		#Sound	xray2 80 33 .9
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	core2
		Type	start
		At	core
		Altpiv   1
		Part	:XrayBeamRed01.part
		#Bhvr	behaviors/XRayBeam.bhvr
		#geom	XrayBeam2
		LifeSpan	12
		POther	T_Chest
		Magnet	T_Chest
		LookAt	T_Chest
	End

	Event
		EName	core3
		Type	start
		At	core
		Altpiv   3
		Part	:XrayBeamRed01.part
		#Bhvr	behaviors/XRayBeam.bhvr
		#geom	XrayBeam2
		LifeSpan	12
		POther	T_Chest
		Magnet	T_Chest
		LookAt	T_Chest
	End

	
End


Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Chest
		Part1	WeaponFX/FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part1	WeaponFX/GernadeLauncherSparks.part
		#Part2	WeaponFX/GernadeLauncherSpark.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Chest
		Part	WeaponFX/FireExplosionFlash.part
		#Part2	WeaponFX/GernadeLauncherSpark.part
		Lifespan 5
		
	End

End
				


Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End




End			