#########################################################
##	
##

FxInfo
LifeSpan  200

Input  
	InpName	origin
End

Input  
	InpName	T_Chest
End

Input  
	InpName	WepR
End


##################################
Condition
	On 	Time
	Time 	31

	Event
		EName 	tracer
		Type	Start
		At	WepR
		Geom	Fx_GunBurst
		Bhvr	Behaviors/ShotGun.bhvr
		LifeSpan  80
	End

	Event
		EName 	fire
		Type	start
		At	WepR
		
		Part	:TorchflameA.part
		Part	:TorchflameB.part
		Part	:SniperFlash.part
		Part	:SniperGlow.part

		LifeSpan	1

	End

	Event
		EName 	tracer2
		Type	start
		At	WepR
		
		sound	cannonfire 100.0 80.0 .9
		LifeSpan	9

		Part	:nemesisMuzzflashCore.part
		Part	:nemesisMuzzflash.part
		Part	:nemesisMuzzflashRing.part
		Part	:nemesisMuzzflashRing2.part
		
		
	End
	
	Event
		EName 	smoke
		Type	start
		At	WepR
		
		LifeSpan	11
		
		Part	:nemesisMuzzflashSmoke.part
			
	End

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Geom	FX_CannonBall
		Bhvr	Behaviors\CannonBallProjectile.bhvr
		Part	:CannonBallSmoke.part
		Magnet T_Chest
		Lookat T_Chest

	End

End

##################################

Condition
	On 	Time
	Time	33

	Event
		Ename 	tracer
		Type	Destroy
	End
End

Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	
	End

End

End				