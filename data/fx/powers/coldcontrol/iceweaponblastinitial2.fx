#########################################################
##	IceBolt
############################
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	WepR
End

Input  
	InpName	Target
End

LifeSpan	200

##################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	local
		At	WepR
		geom	LaserRifle

	End

End

#######################################################

Condition
	On 	Time
	Time 	34

	Event
		Type	Start 
		At	Hookup
		Altpiv	1
		Bhvr	behaviors/powers/coldcontrol/iceWeaponblast.bhvr
		
		Magnet	Target
		LookAt	Target
		
		Part2	:IceWeaponSnowBlast.part
		
		Part3	:IceWeaponMussleFlash2.part
		Part4	:IceWeaponMussleFlash.part
		
	End

	Event
		EName 	mush
		Type	Start 
		At	Hookup
		Altpiv	1
		
		Part1	:HeavyMussleFlash.part
		Part2	:HeavyMussleFlash2.part
		Part3	:MussleSnowStreaks.part
		LifeSpan	3
		
	End
	
End

Condition
	On 	Time
	Time 	38

	Event
		EName 	Prime
		Type	Start 
		At	Hookup
		Altpiv	1
		Bhvr	behaviors/powers/coldcontrol/iceWeaponHeavyblast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail3.part
		Part5	:IceWeaponBlastHeavyTail.part
		Part	:IceWeaponBlastHeavyTail.part
		
		
	End

	Event
		
		Type	local
		At	prime
		
		Magnet	Target
		LookAt	Target
		
		Part2	:IceWeaponBlastTailGlow.part
		
	End

	Event
		EName 	Prime2
		Type	start 
		At	Hookup
		altpiv	5
		Geom	LightDummy2
		bhvr	behaviors/Creyspin3b.bhvr
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	s1
		Type	local 
		At	Prime2
		altpiv	1
		
		Part1	:CreyWeaponStreamer.part
		Part	:IceWeaponBlastTailGlowSmall.part		
	End

	Event
		EName 	s2
		Type	local 
		At	Prime2
		altpiv	2
		
		Part1	:CreyWeaponStreamer.part
		Part	:IceWeaponBlastTailGlowSmall.part		
	End
End

##############################################################
Condition
	On 	Time
	Time 	0



	Event
		EName 	fist
		Type	Local 
		At	origin

		Sound Ice9 100.0 100.0 .75

	End


End


##############################################################


Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

	Event	
		Ename   IceShards
		Type	Posit
		At	target
		Part1	:IceBoltMist.part
		Part2	:ChillTouchSnow.part
		Part3   :ChillingTouchFlash.part

		LifeSpan	20

	End
End

Condition
	On	Prime2Hit
		
	Event
		Ename	Prime2
		Type	Destroy
	End

End


End				