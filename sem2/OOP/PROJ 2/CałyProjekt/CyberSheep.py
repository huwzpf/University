from Animal import Animal
import Constants
import math



class CyberSheep(Animal):

    def __init__(self, position, world, creation_turn):
        super().__init__(Constants.CYBER_SHEEP_STRENGTH, Constants.CYBER_SHEEP_INITIATIVE, position,
                         "cybersheep.png", world, creation_turn)

    def _get_targeted_position(self, hogweeds):
        closest_pos = (0, 0)
        closest_distance = -1
        # find closest hogweed using pitagoras theorem
        for hogweed in hogweeds:
            pos = hogweed.get_position()
            dist = math.sqrt(pos[0]**2 + pos[1]**2)
            if closest_distance == -1 or dist < closest_distance:
                # if closer distance that actually marked as closest was found, replace it
                closest_distance = dist
                closest_pos = pos
        x = self._position[0]
        y = self._position[1]
        if self._position[0] != closest_pos[0]:
            # int((closest_pos[0] - x)/abs(closest_pos[0] - x)) returns -1 if closest hogweed's x position is smaller
            # than CyberSheep's position, 1 otherwise, so sheep always moves by one grid
            x += int((closest_pos[0] - x)/abs(closest_pos[0] - x))
        elif self._position[1] != closest_pos[1]:
            y += int((closest_pos[1] - y)/abs(closest_pos[1] - y))

        return x, y


    def action(self):
        # if there are hogweeds in the world, move towards them, else move randomly just like regular Sheep
        hogweeds_list = self._world.get_hogweeds_positions()
        if len(hogweeds_list) == 0:
            return super().action()
        else:
            return self._get_targeted_position(hogweeds_list)

