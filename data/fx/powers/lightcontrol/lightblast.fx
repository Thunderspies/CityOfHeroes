#########################################################
##	Fire_Ball
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
	Inpname	WepR
End

Input  
	Inpname	WepL
End

####################################################################################



Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepL
		Part1	:LightBlashHandFlash.part
		Part2	:LightBlashHandEmber.part
		Part3	:LightBlastStreaks.part
		Part4	:LightBlastRays.part
		Part5	:LightBlastHandRing.part
	End

	Event
		EName 	Prime2
		Type	Start 
		At	WepL
		Bhvr	behaviors/projectileNOspin.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:LightHead.part
		Part2	:LightHead.part
		Part3	:LightHead.part
		Part4	:LightHead.part
		Part5	:LightHead.part
	End

	Event
		EName	Prime2
		Type	Start
		At	WepL
		Bhvr	behaviors/projectileNOspin.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:RainbowLightHead.part
		Part2	:RainbowLightHead.part
		Part3	:RainbowLightHead.part
		Part4	:RainbowLightHead.part
		Part5	:RainbowLightHead.part

	End

End


Condition
	On 	Time
	Time 	0
	Event
		EName	SwordFlash2
		Type	Local
		At	WepR
		Part1	:LightBlashHandFlash.part
		Part2	:LightBlashHandEmber.part
		Part3	:LightBlastStreaks.part
		Part4	:LightBlastRays.part
		Part5	:LightBlastHandRing.part

	End

End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	behaviors/projectileNOspin.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:LightHead.part
		Part2	:LightHead.part
		Part3	:LightHead.part
		Part4	:LightHead.part
		Part5	:LightHead.part
	End

	Event
		EName	Prime
		Type	Start
		At	WepR
		Bhvr	behaviors/projectileNOspin.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:RainbowLightHead.part
		Part2	:RainbowLightHead.part
		Part3	:RainbowLightHead.part
		Part4	:RainbowLightHead.part
		Part5	:RainbowLightHead.part

	End

	
End

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

	Event
		Ename	Prime2
		Type	Destroy
	End
		
	Event
		Ename	FireBallHit
		Type	local
		At	target

		#Part1	:BigFireExplosion.part
		#Part2	:FireBallHitSpark.part
	End

End

End	


			