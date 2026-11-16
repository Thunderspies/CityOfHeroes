#########################################################
##	
##

FxInfo
LifeSpan  60

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
InitialVelocity		0 0 0
InitialVelocityJitter	0 0 0
			physgravity		0
			PyrRotate	-90 0 0
			scale		.1 .1 .1
		end
		lifespan 60
	End	


end


End				