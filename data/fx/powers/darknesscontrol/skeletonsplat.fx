#########################################################
##	chill touch hit
FxInfo

##################################

Condition
	On	Time
	Time	0

	Event	

		ENAME	hookupW/Spin
		Type	posit
		At	origin
		
		BHVR	:SplatRotate.bhvr
	End

End

Condition
	On	cycle
	Time	5

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:Darktendril.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30
		
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarktendrilIn.bhvr

		Splat	DarknessSplat2.tga

		LifeSpan	30
	End
	
	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarkSplat2.bhvr

		Splat	Ember02.tga
		LifeSpan	30
	End

End

Condition
	On	cycle
	Time	19

	Event	

		ENAME	BaseTrail
		Type	start
		At	origin

		Bhvr	:splat.bhvr
		splat	Generic_Ring.tga
		LifeSpan	30
	End


End


End			

##################################
