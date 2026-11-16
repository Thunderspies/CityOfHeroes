#########################################################
##	
##

FxInfo
LifeSpan  110

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End
##################################
Condition
	On 	Time
	Time 	80

	Event
		EName 	Blast
		Type	local
		At	WepR
		geom	Nemesis_Rifle
		
	End

	Event
		EName 	Prime
		Type	Start
		At	Blast
		Altpiv	1
		Geom	FX_CannonBall
		Bhvr	Behaviors\CannonBallProjectile.bhvr
		Part	:CannonBallSmoke.part
		Magnet T_Chest
		Lookat T_Chest
	End

	Event
		EName 	tracer
		Type	Start
		At	Blast
		Altpiv	1
		Geom	Fx_GunBurst
		Bhvr	Behaviors/ShotGun.bhvr
	End

	Event
		EName 	fire
		Type	start
		At	Blast
		Altpiv	1
		
		Part	:TorchflameAblue.part
		Part	:TorchflameBblue.part
		Part	:SniperFlashblue.part
		Part	:SniperGlowblue.part

		LifeSpan	1

	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	2
		
		sound	shotgun3 120.0 30.0 .9
		LifeSpan	9

		Part	:nemesisMuzzflashCore.part
		Part	:nemesisMuzzflash.part
		Part	:nemesisMuzzflashRing.part
		Part	:nemesisMuzzflashRing2.part
		
		
	End
	
	Event
		EName 	smoke
		Type	start
		At	Blast
		Altpiv	2
		
		LifeSpan	11
		
		Part	:nemesisMuzzflashSmoke.part
			
	End

End


##################################



Condition
	On 	Time
	Time	86

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