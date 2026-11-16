#########################################################
##	
##

FxInfo
LifeSpan  100

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
	Time 	39
	
	Event
		Type	Local
		At 	origin
		Sound Arachnosbot_blast2 100 100 .8
	End
End



Condition
	On 	Time
	Time 	0 #38


	Event
		ENAME   cannon
		Type	local
		At	hair
		bhvr	:Bot_Blaster_off.bhvr
#		geom    Testing_Target
#		Sound	Assault3 120.0 30.0 1.0
#		magnet	t_chest
#		lifespan 3
	End


	Event
		ename	genspot
		Type	local
		At	waist
		Sound ArachnosBot_servo3 120.0 120.0 1.0

		bhvr	:Bot_Blaster_Genspot.bhvr
	end

	Event
		ename	gens
		Type	local
		At	waist

		bhvr	:Bot_Blaster_Gens.bhvr
		part	:bot_blaster_gensparksBIG.part
		#geom    Testing_Target
	end

	Event
		Type	local
		At	cannon
		part	:bot_blaster_chargeball.part
		lifespan 55
	end


	
	Event
		EName 	suctionR
		Type	local
		At	waist
		bhvr	:Bot_Blaster_BubbleOff.bhvr
#		geom    Testing_Target
		Lifespan 28
		PMagnet	waist
	End	
	Event
		EName 	suctionL
		Type	local
		At	waist
		bhvr	:Bot_Blaster_BubbleOffN.bhvr
#		geom    Testing_Target
		Lifespan 28
		PMagnet	waist
	End

	
	Event
		Type	local
		At	suctionR
		Part1	:bot_blaster_Bubbles.part
		Part2	:bot_blaster_BubblesWhite.part
		PMagnet	waist
	End
	
	Event
		Type	local
		At	suctionL
		Part1	:bot_blaster_Bubbles.part
		Part2	:bot_blaster_BubblesWhite.part
		PMagnet	waist
	End

#	Event
#		EName 	HandGlowLeft1
#		Type	local
#		At	WepL
#		Part1	:bot_blaster_Bubbles.part
#		Part2	:bot_blaster_BubblesWhite.part
#		
#		PMagnet	WepL
#	End
	


end

Condition
	On 	time
	Time 	38




	Event
		ENAME   prime
		Type	start
		At	cannon
		bhvr	:Bot_Blaster.bhvr
#		geom    Testing_Target
		Part1	:Bot_Blaster_BallTailBig.part
		Part1	:Bot_Blaster_ShotBallGlow.part
#		Part1	:Bot_Blaster_BallStar.part
		Part1	:Bot_Blaster_SteamTrail.part
		magnet	t_chest
		
		lifespan 60
#		lifespan 3
	End


	Event
		Type	start
		At	prime

		Part1	:Bot_Blaster_ShotRing.part
		Part1	:Bot_Blaster_ShotRing.part
	end


	Event
		ENAME   screw
		Type	local
		At	prime

		bhvr	:Bot_Blaster_Twist.bhvr
	end

	Event
		Type	local
		At	screw

		bhvr	:Bot_Blaster_TwistOff.bhvr
		Part1	:Bot_Blaster_BallTwist.part
		Part1	:Bot_Blaster_BallTwist.part
		lifespan 60
	end


	Event
		Type	local
		At	screw

		bhvr	:Bot_Blaster_TwistOff2.bhvr
		Part1	:Bot_Blaster_BallTwist.part
		Part1	:Bot_Blaster_BallTwist.part
		lifespan 60
	end
	Event
		Type	local
		At	genspot
		part1	:Bot_Blaster_ringSmoke.part
		part1	:Bot_Blaster_ringSmokeN.part
		part1	:Bot_Blaster_ringSteam.part
		part1	:Bot_Blaster_ringSteamN.part
		pmagnet root
	end
#	Event
#		Type	local
#		At	genspot
#		part1	:Bot_Blaster_ringSmoke.part
#		part1	:Bot_Blaster_ringSmokeN.part
#		part1	:Bot_Blaster_ringSteam.part
#		part1	:Bot_Blaster_ringSteamN.part
#		pmagnet root
#		lifespan 10
#	end

End	
	



Condition
	On 	primehit
	
	Event
		EName 	Prime
		Type	Destroy
		

	End
end
