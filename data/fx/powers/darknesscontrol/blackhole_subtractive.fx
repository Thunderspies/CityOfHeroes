#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	T_root
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

LifeSpan	180
##################################
Condition
	On	Time
	Time	0
	
	Event	

		ENAME	hookupW/Spin
		Type	posit
		At	T_root
		
		BHVR	:SplatRotate.bhvr
		Sound Tar_loop 80 80 .53
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	T_root

		Bhvr	:BlackHoleSplat.bhvr
		splat	Generic_Ring.tga
		LifeSpan	10
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	T_root

		Part	:CreateDarknessPuddle3_Subtractive.part

	End

End

Condition
	On	cycle
	Time	25
	Chance	.5

	Event	

		ENAME	BaseRings
		Type	start
		At	T_root

		Bhvr	:BlackHoleSplat.bhvr
		splat	Generic_Ring.tga
		LifeSpan	10
	End


End

Condition
	On	cycle
	Time	15

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarktendrilInDark.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:Darktendril.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30
		
	End

	Event	

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarktendrilIn.bhvr

		Splat	DarknessSplat2.tga

		LifeSpan	30
	End
	
	Event	

		ENAME	BaseTrail2
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarkSplat2.bhvr

		Splat	Ember02.tga
		LifeSpan	30
	End

End

Condition
	On	Time
	Time	20
	
	Event
		ENAME	Base1
		Type	local
		At	WepR

		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part

	End

	Event	

		ENAME	Base2
		Type	local
		At	WepL
		
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
	End

	
End

Condition
	On 	Time
	Time	110

	Event
		EName 	Base1
		Type	Destroy

	End

	Event
		EName 	Base2
		Type	Destroy

	End

End

End			