#########################################################
##	
##

FxInfo
LifeSpan  90

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
	On 	time
	Time 	0

	Event
		ENAME   splinter1
		Type	start
		At	origin
		bhvr	:BS_Tail_Blast_Hit.bhvr
		Part1	:BS_Tail_BallTailBig.part
		Part1	:BS_Tail_ShotBallGlow.part
		Part1	:BS_Tail_SteamTrail.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		bhvroverride
			PyrRotate	-90 0 0
			scale		.1 .1 .1
		end
		CEvent	:BS_Tail_blast_ShrapnelFade.fx
		CThresh	0.000001
		CDestroy	1
		lifespan 45

	End	




	Event
		ENAME   splinter2
		Type	start
		At	origin
		bhvr	:BS_Tail_Blast_Hit.bhvr
		Part1	:BS_Tail_BallTailBig.part
		Part1	:BS_Tail_ShotBallGlow.part
		Part1	:BS_Tail_SteamTrail.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		bhvroverride
			PyrRotate	-90 0 0
			scale		.1 .1 .1
		end
		CEvent	:BS_Tail_blast_ShrapnelFade.fx
		CThresh	0.000001
		CDestroy	1
		lifespan 60
	End	



	Event
		ENAME   splinter3
		Type	start
		At	origin
		bhvr	:BS_Tail_Blast_Hit.bhvr
		Part1	:BS_Tail_BallTailBig.part
		Part1	:BS_Tail_ShotBallGlow.part
		Part1	:BS_Tail_SteamTrail.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		bhvroverride
			PyrRotate	-90 0 0
			scale		.1 .1 .1
		end
		CEvent	:BS_Tail_blast_ShrapnelFade.fx
		CThresh	0.000001
		CDestroy	1
		lifespan 75
	End	

	Event
		ENAME   splinter3
		Type	start
		At	origin
		bhvr	:BS_Tail_Blast_Hit.bhvr
		Part1	:BS_Tail_BallTailBig.part
		Part1	:BS_Tail_ShotBallGlow.part
		Part1	:BS_Tail_SteamTrail.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		part1	:BS_Tail_ShrapnelSmoke.part
		bhvroverride
			PyrRotate	-90 0 0
			scale		.1 .1 .1
		end
		CEvent	:BS_Tail_blast_ShrapnelFade.fx
		CThresh	0.000001
		CDestroy	1
		lifespan 90
	End	




end


End				