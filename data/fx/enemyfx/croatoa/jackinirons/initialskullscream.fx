################x#########################################
##	chill touch hit
FxInfo

LifeSpan	105	#75

############################################################

Condition
	On 	Time
	Time 	60	#30

	
	Event
		EName	PrimeBlast
		Type	local
		At	WepL
		Bhvr	:SkullScreamOffset.bhvr

		Part	:SkullRingsBlackScream.part
		Part	:SkullRingsScream.part
		Part	:SkullRingsScream2.part
		Part	:SkullRingsBlackScreamBig.part
		Part	:SkullRingsBlackScreamBigthin.part
		Sound JackScream1 100 100 .9
		LifeSpan	30
	
	End

End

End




















		