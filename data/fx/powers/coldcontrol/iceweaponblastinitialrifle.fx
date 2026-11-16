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
		geom	energyRifle

	End

End

#######################################################

Condition
	On 	Time
	Time 	33

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
		
		Sound Ice9 100.0 100.0 .75
	End
	
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Prime
		Type	Start 
		At	Hookup
		Altpiv	1
		Bhvr	behaviors/powers/coldcontrol/iceWeaponblast.bhvr
		Geom	FX_IceBlast
		Magnet	Target
		LookAt	Target
		Part3	:IceBoltSnowtrail2.part
		Part4	:IceBoltMistTrail3.part
		Part5	:IceWeaponBlastTail.part
		Part	:IceWeaponBlastTail.part
	End

	Event
		
		Type	local
		At	prime
		
		Magnet	Target
		LookAt	Target
		
		Part2	:IceWeaponBlastTailGlow.part
		
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


End				