#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	Origin
End


Lifespan	130

##################################

Condition
	On	Time
	Time	0	

	Event
		Type	start
		At	Origin
		part	:FlashFreezeCubes.part
		Geom	FX_IceBlast 
	End

	Event
		EName	Hookup
		Type	local
		At	Origin
		Geom	Fx_IceWEapon #OverHeadDummy01
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		Sound icerain 100 100 .86
	End

	Event
		Type	local
		At	Hookup
		altpiv	1
		
		#part	:FreezingRainA.part
		#Part	:FreezingRains.part
		#Part	:FreezingSteam.part
		#part	:FreezingRainA.part
		#Part	:FreezingRains.part
		#Part	:FreezingSteam.part

		Geom	FX_IceWeapon
		#Bhvr	Behaviors/GenericParticleFade.bhvr
		
	End


End

Condition
	On 	Time
	Time	120

	Event
		EName 	LastRainFalling
		Type	destroy
	End
	
End	



End		