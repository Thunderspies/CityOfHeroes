#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

Input  
	InpName	head
End

##################################
Condition
	On	Time
	Time	0
	
	Event
		EName	p2
		Type	Local
		At	head
		geom	telescope
		bhvr	behaviors/TelescopeScale.bhvr
	End

End



Condition
	On	Time
	Time	0

	Event
		EName	Rays1
		Type	Local
		At	p2
		 AltPiv 1

		Part4	:TelescopeRainbowRing.part

	End

End


Condition
	On	Time
	Time	10

	Event
		EName	Rays1
		Type	Local
		At	p2
		 AltPiv 1

		Part4	:TelescopeRainbowRing2.part

	End

End


Condition
	On	Time
	Time	5

	Event
		EName	Rays2
		Type	Local
		At	p2
		 AltPiv 1

		Part1	:TelescopeLightRays.part
		Part2	:TelescopeLightRing.part
		Part3	:TelescopeStreaks.part
		#Part4	:TelescopeRainbowRing.part

	End

End


End				