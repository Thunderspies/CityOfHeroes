#########################################################
##	
##

FxInfo
LifeSpan  180

Input  
	InpName	Target
End

Input  
	InpName	WepR
End


##################################

Condition
	On	Time
	Time	43

	Event
		Type	Local
		At	origin
		sound	angelfire1 90.0 78.0 .9
	End
End



Condition
	On 	Time
	Time 	47

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
		#Geom	FX_CannonBall
		Bhvr	Behaviors\CannonBallProjectile.bhvr
		#Part	WEAPONFX\CannonBallSmoke.part
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
		
		Part	WEAPONFX\TorchflameAblue.part
		Part	WEAPONFX\TorchflameBblue.part
		Part	WEAPONFX\SniperFlashblue.part
		Part	WEAPONFX\SniperGlowblue.part

		LifeSpan	1

	End

	Event
		EName 	tracer2
		Type	start
		At	Blast
		Altpiv	2
		
		
		LifeSpan	9

		Part	WEAPONFX\nemesisMuzzflashCore.part
		#Part	WEAPONFX\nemesisMuzzflash.part
		Part	WEAPONFX\nemesisMuzzflashRing.part
		Part	WEAPONFX\nemesisMuzzflashRing2.part
		
		
	End
	
	Event
		EName 	smoke
		Type	start
		At	Blast
		Altpiv	2
		
		LifeSpan	11
		
		Part	WEAPONFX\nemesisMuzzflashSmoke.part
			
	End

	Event
		EName 	Blast1
		Type	start
		At	Blast
		Altpiv 1
		
		
		#Sound	flamethrower2 120.0 30.0 .95
		
		Part1	:FlameThrowerFlame.part		
		Part	:FireSpecksSpit.part
		Part	:FireSpecksFalling.part
		Part	:FireSpit2.part
		Part	:FireSpitStart.part
		
		Part	:FireLicks.part
	End

	Event
		EName 	Blast2
		Type	local
		At	Blast
		Altpiv 1
		
		Part1	:TorchflamethrowerA.part
		
	End

	Event
		EName 	Blast3
		Type	local
		At	Blast
		Altpiv 1
		
		
		Part2	:TorchflamethrowerB.part
	End

End

##################################

Condition
	On 	Time
	Time	49

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

Condition
	On 	Time
	Time 	60  ##150

	Event
		EName 	Blast2
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	54 ##164

	Event
		EName 	Blast3
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	62  ##170

	Event
		EName 	Blast1
		Type	Destroy
				
	End
End

End				