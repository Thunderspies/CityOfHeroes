#########################################################
##	General red electricity crawling across characters limbs
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

LifeSpan	120

########################################################





Condition

	On 	time
	Time 	0
	Event
		Type	start 
		ename	AnchorR
		At	root
		BhvrOverride
			PositionOffset	.54 94 -1.27
		end
		part1	:Spider_Web_Stretch.part
#		Geom	Testing_Target
		pother	ToeR
	end

	Event
		Type	start 
		ename	AnchorL
		At	root
		BhvrOverride
			PositionOffset	-.54 94 -1.27
		end
		part1	:Spider_Web_Stretch.part
		Sound call_RIF_bot_descend 200 200 .85
#		Geom	Testing_Target
		pother	ToeL
	end


	Event
		Type	positonly 
		ename	SpinneretteR
		At	Hair
		BhvrOverride
			PositionOffset	.05 1 -1.238
			Stretch 1
		end
		part1	:Spider_Web_Spinerette.part
		part1	:Spider_Web_Spinerette.part
		part1	:Spider_Web_Spinerette.part
#		Geom	Testing_Target
		pother	ToeR
		lookat	ToeR
		pmagnet ToeR
	end

	Event
		Type	positonly 
		ename	SpinneretteL
		At	Hair
		BhvrOverride
			PositionOffset	-.05 1 -1.238	
			Stretch 1
		end
		part1	:Spider_Web_Spinerette.part
		part1	:Spider_Web_Spinerette.part
		part1	:Spider_Web_Spinerette.part
#		Geom	Testing_Target
		pother	ToeL
		lookat	ToeL
		pmagnet ToeL
	end

#	Event
#		Type	positonly 
#		ename	ToezorR
#		At	ToeR
#		BhvrOverride
##			PositionOffset	.05 1 -1.238
#			Stretch 1
#		end
#		lookat	SpinneretteR
#	end
#
#	
#	Event
#		Type	positonly 
#		ename	ToezorL
#		At	ToeL
#		BhvrOverride
##			PositionOffset	.05 1 -1.238
#			Stretch 1
#		end
#		lookat	SpinneretteL
#	end
#
end

#Condition
#
#	On 	time
#	Time 	1
#	Event	
#		Ename	windy
#		Type	local 
#		At	 ToezorR
#		part1	:Spider_Web_Spinerette.part
#		pother	SpinneretteR
#	end
#	Event	
#		Ename	windy
#		Type	local 
#		At	 ToezorL
#		part1	:Spider_Web_Spinerette.part
#		pother	SpinneretteL
#	end
#end

Condition

	On 	time
	Time 	53
	
	Event
		Type	start 
		At	ToeR
		part1	:Spider_Web_Billow.part
		part1	:Spider_Web_Billow.part
		part1	:Spider_Web_Billow.part
		part1	:Spider_Web_Billow.part
		BhvrOverride
			trackrate 100
		end
		pother	AnchorR
		magnet	AnchorR
		pmagnet	windy
		Sound call_RIF_bot_land 100 100 .7

	end

	Event
		Type	start 
		At	ToeL
		part1	:Spider_Web_Billow.part
		part1	:Spider_Web_Billow.part
		part1	:Spider_Web_Billow.part
		part1	:Spider_Web_Billow.part
		BhvrOverride
			trackrate 100
		end
		pother	AnchorL
		magnet	AnchorL
		pmagnet	windy
	end
End

Condition

	On 	time
	Time 	52

	Event	
		Ename	windy
		Type	start 
		At	root
		BhvrOverride
			PositionOffset	0 35 0
			StartJitter		30 0 30
#			Gravity			-0.01
			InitialVelocityJitter	3 1 3
			scale 10 10 10
		end
#		Geom	Testing_Target
	end

	Event
		Type	local 
		At	ToeR
		part1	:Spider_Web_TipSpray.part
		part1	:Spider_Web_TipSpray.part
		part1	:Spider_Web_TipSpray.part
		part1	:Spider_Web_TipSpray.part
#		pother	AnchorR
#		magnet	AnchorR
		pmagnet	windy
		lifespan 15
	end

	Event
		Type	local 
		At	ToeL
		part1	:Spider_Web_TipSpray.part
		part1	:Spider_Web_TipSpray.part
		part1	:Spider_Web_TipSpray.part
		part1	:Spider_Web_TipSpray.part
#		pother	AnchorR
#		magnet	AnchorR
		pmagnet	windy
		lifespan 15
	end

end


Condition

	On 	time
	Time 	61
	Event
		Type	destroy 
		ename	SpinneretteR

	end
	Event
		Type	destroy 
		ename	SpinneretteL

	end


#	Event
#		Type	start 
#		At	ToeR
#		part1	:Spider_Web_Billow.part
#		part1	:Spider_Web_Billow.part
#		part1	:Spider_Web_Billow.part
#		part1	:Spider_Web_Billow.part
#		BhvrOverride
#			trackrate 100
#		end
#		pother	SpinneretteR
#		magnet	SpinneretteR
#		pmagnet	windy
#	end
#
#	Event
#		Type	start 
#		At	ToeL
#		part1	:Spider_Web_Billow.part
#		part1	:Spider_Web_Billow.part
#		part1	:Spider_Web_Billow.part
#		part1	:Spider_Web_Billow.part
#		BhvrOverride
#			trackrate 100
#		end
#		pother	SpinneretteL
#		magnet	SpinneretteL
#		pmagnet	windy
#	end

end
