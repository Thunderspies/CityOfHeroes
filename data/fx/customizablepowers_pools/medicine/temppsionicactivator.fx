#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	wepL
End

Lifespan 160

#########  Tricorder prop  ###########################################

Condition
	On	Time
	Time	20

	Event
		Type	local
		At	Origin
		Sound med3 60 60 .5

	End
End

Condition
	On	Time
	Time	29

	Event
		Type	local
		At	Origin
		Sound mental9 60 60 .73

	End
End
######################################################################

Condition
	On	Time
	Time	20
	
	Event
		EName	Pod
		Type	Local
		At	WepL
		Geom	GEO_WepL_MediPod
##		Bhvr	Behaviors/PsionicPodFadeIn.bhvr
		Part	:Pglow.part
		
		Lifespan 100
	
	End


End

##########################################################################################

Condition
	On	Time
	Time	55
	
	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers/Psionics/PsionicMWaves01.part
		part2	Powers/Psionics/PsionicMCloud01.part
		part3	:PGlow.part

		Lifespan 33
	End
	
	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers/Psionics/PsionicMWaves02.part
		Lifespan 33
		Pmagnet Pod
		
	End
	
	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers/Psionics/PsionicMStreaks01.part
		Lifespan 25
	End
	
	Event
		EName	Ring
		Type	Local
		At	Pod
		Altpiv	1
		part1	Powers/Psionics/PsionicMAidBeam01.part
		part2	Powers/Psionics/PsionicBeam02.part
		Lifespan 33
	End

End

End